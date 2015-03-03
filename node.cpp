#include "node.h" 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
Node::Node(string name){
  degree = 0;
  cc = 0;
  cc_max = 0;
  cc_min = 0;
  nb_top_neighbours= 0;
  title = name;
  neighbours_indexs = set<int>();
}

 

void Node::addneighbours(Node* son){
 unordered_map<int,Node*>::const_iterator it = neighbours.find(son->get_index());
  if(it == neighbours.end()){
    neighbours[son->get_index()]=son; 
    neighbours_indexs.insert(son->get_index());
    degree++; 
  }else{
  }
}



void Node::calculate_redundancy(){
  // double nb = 0;
  // set<int>::iterator it;
  // for(it = neighbours_indexs.begin() ; it != neighbours_indexs.end() ; it++){
  //   Node * n_son = neighbours[*it];
  //   set<int>::iterator it2;
  //   for(it2 = it ; it2 != neighbours_indexs.end() ; it2++){
  //     if(it != it2){
  //       vector<int> tmp;
  //       Node * n_son_2 = neighbours[*it2];
  //       std::set_intersection(n_son->neighbours_indexs.begin(), n_son->neighbours_indexs.end(),
  //                         n_son_2->neighbours_indexs.begin(), n_son_2->neighbours_indexs.end(),
  //                         std::back_inserter(tmp));
  //       if(tmp.size() > 1){
  //         nb++;
  //       }
  //     }
  //   }
  // }
  // cout << pairs.size() << "   " << nb << "  \n";
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

      