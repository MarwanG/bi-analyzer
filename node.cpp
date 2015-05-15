#include "node.h" 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

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
  max_interval=0;
  max_packet = 0;
  nb_video_packs=0;
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


int Node::get_total_up(){
  map<int,int>::iterator it;
  int total = 0;
  for(it = size_pack_list_total.begin() ; it != size_pack_list_total.end() ; it++){
    total = total + it->second;
  }
  return total;
}


int Node::get_total_down(){
  map<int,int>::iterator it;
  int total = 0;
  for(it = size_pack_list_total_down.begin() ; it != size_pack_list_total_down.end() ; it++){
    total = total + it->second;
  }
  return total;
}

void Node::add_pack_down(Node*son,int size_pack){
  size_pack_list_down[son->get_index()].push_back(size_pack);
  size_pack_list_total_detail[son->get_index()].push_back(size_pack);
  size_pack_list_total_down[son->get_index()] = size_pack_list_total[son->get_index()] + size_pack;
  if(son->get_title().compare("1.180.112.60")==0){
    cout << son->get_title() << "  " << son->max_packet << "  " << size << "\n";
  }
  if(son->max_packet < size_pack){
    son->max_packet = size_pack;
  }
  if(size_pack > 999){
    son->nb_video_packs++;
  }else{
    son->nb_signalling_packs++;
  }
}


void Node::add_pack_up(Node*son,int size_pack){
  size_pack_list[son->get_index()].push_back(size_pack);
  size_pack_list_total_detail[son->get_index()].push_back(size_pack);
  size_pack_list_total[son->get_index()] = size_pack_list_total[son->get_index()] + size_pack;
  if(son->get_title().compare("1.180.112.60")==0){
    cout << son->get_title() << "  " << son->max_packet << "  " << size << "\n";
  }
  if(son->max_packet < size_pack){
    son->max_packet = size_pack;
  }
  if(size_pack > 999){
    son->nb_video_packs++;
  }else{
    son->nb_signalling_packs++;
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

