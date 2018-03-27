#include <utility>
#include <iterator>
#include <string.h>
#include <map>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<bits/stdc++.h>
std::map<std::string, size_t> d_full;

std::string transformer(std::string word){
    std::string new_word;
    for(int i = 0; i < word.length(); i++){
        if(word[i] != '.' && word[i] != ',' && word[i] != '?' && word[i] != '!' && word[i] != ';' && word[i] != ':' && word[i] != '"'){
            char w = word[i];
            w = tolower(w);
            new_word += w;
        }
    }
    return new_word;
}


bool sorter(const std::pair<std::string, size_t> &a,
              const std::pair<std::string, size_t> &b)
{
    return (a.second > b.second);
}


inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}


template<class D>
inline long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::seconds>(d).count();
}


bool fexists(const char *filename) {
  std::ifstream ifile(filename);
  return (bool)ifile;
}


std::vector<std::string> get_args(std::string f){
    std::ifstream conf(f);
    std::string name;
    std::vector<std::string> names;
    while(conf >> name){
        std::string filename;
        bool started = false;
        bool numb = false;
        for (int i = 0; i < name.size(); i++){
            if (i > 0 && name[i] == '"' && name[i - 1] == '='){
                started = true;
            }
            else if(i > 0 && name[i] != '"' && name[i - 1] == '='){
				numb = true;
			}
			if(started == true && name[i] != '"'){
                filename += name[i];
            }
			else if(numb == true){
				filename += name[i];
			}
        }
		names.push_back(filename);
    }
	return names;
}

std::map<std::string, size_t> mapper(int start, int end, std::vector<std::string> everything, std :: mutex& m){
	//std::vector<std::string> part_of_all;
	std::map<std::string, size_t> d;
    for (int i = start; i < end; i++){
		//part_of_all.push_back(transformer(everything[i]));
		m.lock();
		++d_full[transformer(everything[i])];
		m.unlock();	
	}
	//std::cout << d.size() << std::endl;
	return d; 
}	

int main(int argc, char* argv[]){
	std::mutex m;
	bool er = false;
    std::string infile;
	std::string out_by_a;
	std::string out_by_n;
	int nthreads;
    std::string conf_file;
    if(argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)){
        std::cout << "counter.cpp\nv1.0\nMade by Roman Stepaniuk and Arsen Tymchyshyn\nCounts number of occurrences of the words from the file and shows time of execution" << std::endl;
        std::cout << "Argument should be name of txt file with arguments or <-auto> and default file conf.txt will be used\nOtherwise default arguments will be used" << std::endl;
        return 0;
    }
    else if(argc == 2){
        if(fexists(argv[1]) == 1){
            conf_file = argv[1];
        }
        else if(strcmp(argv[1], "-auto") == 0) {
            conf_file = "conf.txt";
        }
		else{
			er = true;
		}
		std::vector<std::string> gotem = get_args(conf_file);
		if(gotem.size() == 4 && er == false){
			infile = gotem[0];
			out_by_a = gotem[1];
			out_by_n = gotem[2];
			nthreads = atoi(gotem[3].c_str());
		}
      	
    }
    else{
		er = true;
	}
	if(er){
        infile = "data.txt";
		out_by_a = "res_a.txt";
		out_by_n = "res_n.txt";
		nthreads = 4;
    }
    auto stage1_start_time = get_current_time_fenced();
    std::string word;
    std::ifstream textfile(infile);
    std::map<std::string, size_t> d;
	std::vector<std::string> everything;
  	
    while(textfile >> word){
        everything.push_back(word);
    }
    auto stage2_start_time = get_current_time_fenced();
    std::cout << "Loading: " << to_us(stage2_start_time - stage1_start_time) << " sec" << std::endl;
  	double dif = everything.size() / nthreads;
  	int start = 0;
  	int end = dif;
	std::vector<std::thread> threads;
  	for(int i = 0; i < nthreads; ++i){
      	if(i == nthreads - 1){
			end = everything.size();
		}
    	threads.emplace_back(mapper, start, end, everything, std::ref(m));
		start += dif;
		end += dif;
    }
	for(auto& thread : threads){
		thread.join();
	}
    auto stage3_start_time = get_current_time_fenced();
    std::cout << "Analyzing: " << to_us(stage3_start_time - stage2_start_time) << " sec" << std::endl;
    std::ofstream wrfile1(out_by_a);
    std::ofstream wrfile2(out_by_n);
    std::vector<std::pair<std::string, size_t> > pairs;
    std::map <std::string, size_t> :: iterator itr;
    for (itr = d_full.begin(); itr != d_full.end(); ++itr)
    {
        pairs.push_back(*itr);
        wrfile1 << '\t' << itr->first 
              << '\t' << itr->second << '\n';
    }
    sort(pairs.begin(), pairs.end(), sorter);
    for (int i = 0; i < pairs.size(); i++){
        wrfile2 << '\t' << pairs[i].first 
              << '\t' << pairs[i].second << '\n';
    }
    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - stage1_start_time;
    std::cout << "Total: " << to_us(total_time) << " sec" << std::endl;
    return 0;
}
