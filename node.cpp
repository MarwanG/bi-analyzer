#include "node.h" 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "print_tools.h"

using namespace std;
Node::Node(string name){
  degree = 0;
  cc = 0;
  cc_max = 0;
  cc_min = 0;
  nb_top_neighbours= 0;
  title = name;
  neighbours_indexs = set<int>();
  freq_ping = map<int,vector<double> >();
  total_packs=0;
}

 

bool Node::addneighbours(Node* son){
  unordered_map<int,Node*>::const_iterator it = neighbours.find(son->get_index());
  if(it == neighbours.end()){
    neighbours[son->get_index()]=son; 
    neighbours_indexs.insert(son->get_index());
    degree++; 
    return true;
  }else{
    return false;
  }
}


// void Node::add_pack(Node*son,int size_pack){
  
// }


void Node::add_ping(Node *son,string s,int size_pack){ 
  if(freq_ping.find(son->get_index()) == freq_ping.end()){
    vector<double> tmp;
    tmp.push_back(0);
    freq_ping[son->get_index()] = tmp;
    freq_last_time[son->get_index()] = s;
    size_pack_list[son->get_index()] = size_pack;
  }else{
    string prev_time = freq_last_time[son->get_index()];
    time_t t1 = timestamp_to_ctime(prev_time.c_str());
    time_t t2 = timestamp_to_ctime(s.c_str());
    double t = difftime(t1,t2);
    

    string t1_str_milli = s.substr(s.find('.')+1,s.length());
    if(t1_str_milli[0]=='0'){
      t1_str_milli = s.substr(s.find('.')+2,s.length());
    }
    string t2_str_milli = prev_time.substr(prev_time.find('.')+1,prev_time.length());
    if(t2_str_milli[0]=='0'){
      t2_str_milli = prev_time.substr(prev_time.find('.')+2,prev_time.length());
    }
    int t1_milli = atoi(t1_str_milli.c_str());
    int t2_milli = atoi(t2_str_milli.c_str());
    int diff_milli_tmp = abs(t1_milli-t2_milli);
    t = t * 1000;
    t = t + diff_milli_tmp;
    freq_ping[son->get_index()].push_back(t);
    freq_last_time[son->get_index()] = s;
    size_pack_list[son->get_index()] = size_pack_list[son->get_index()] + size_pack;
  }
}


void Node::calculate_redundancy(){
  double nb = (double) pairs.size();
  double bot = (degree*(degree-1))/2;
  if(bot != 0 ){
    red = nb/bot;
  }else{
    red = -1;
  }
}


void Node::calculate_disp(){
  int sum = 0 ;
  set<int>::iterator it;
  for(it = neighbours_indexs.begin() ; it != neighbours_indexs.end() ; it++){
    sum += (neighbours[*it]->get_degree()-1);
  }
  if(sum == 0){
    disp = -1;
  }else{
    disp = (float)nb_top_neighbours/(float)sum;
  }
}



// SETTERS AND GETTERS

double Node::get_degree(){
  return degree;
}


string Node::get_title(){
  return title;
}

int Node::get_index(){
  return index;
}


void Node::add_cc(float new_cc){
  cc+=new_cc;
}
void Node::add_cc_min(float new_cc){
  cc_min+=new_cc;
}
void Node::add_cc_max(float new_cc){
  cc_max+=new_cc;
}

void Node::set_index(int i ){
  index = i;
}


float Node::get_cc(){
  return cc;
}
  
float Node::get_cc_min (){
  return cc_min;
}
  

void Node::set_cc(float cc_new){
  cc= cc_new;
}


void Node::set_cc_min(float cc_new){
  cc_min= cc_new;
}


void Node::set_cc_max(float cc_new){
  cc_max= cc_new;
}


float Node::get_cc_max (){
  return cc_max;
}

void Node::set_red(float r){
  red = r;
}

float Node::get_red(){
  return red;
}

float Node::get_disp(){
  return disp;
}

// float Node::get_red_norm(){
//   return red_norm;
// }
