#ifndef Sudoku_HH
#define Sudoku_HH

#include <iostream>
#include <fstream>
//#include <cstdlib>
#include <string>
//#include <iomanip>
#include <bitset>
#include <vector>
#include <stack>


#ifdef VERBOSELOG
#define DLOG(x) x
#else
#define DLOG(x)
#endif

using namespace std;

class bSudoku  {
private:	
	bSudoku(const bSudoku &);
    bSudoku& operator=(const bSudoku &);

protected:
	
public:
    static const int SIZE = 9;
	bSudoku();
	~bSudoku();
	bSudoku* new_hypothesis();
	bSudoku* Clone();
	bool is_solved();
	bool elimination();
	bool is_impossible();
	void disp_used();
	void disp_t_hyp();
	void disp_flags();

	static const int FLAGSSIZE = 2;
	static const int GFLAGSSIZE = 1;
	static const int CUADRANT = 3;
	enum FLAG { done, hyp_tried }; 
	enum GFLAG { no_hyp_left };

	bitset<SIZE>** used; // FIXME change to pos
	
	// done is for a definitive element
	// hyp_tried means all the bits have been tried for hypothesis
	bitset<FLAGSSIZE>** flags; // FIXME

	bitset<SIZE>** t_hyp; // tried hypothesis
	
	// flags of the whole bSudoku
	bitset<GFLAGSSIZE> gflags;
};



class Sudoku : public bSudoku {
private:
	short** data;
	short** result;
public:
	Sudoku();
	Sudoku(bSudoku* sudoku);
	~Sudoku();
	bool read_from_file(const string & file);
	void initialize();
	void disp_data();
	void disp_result();
	void calc_used();
};


class Node {
public:	
	Node()
	{
		this->bsudoku = 0;
		possible = true;
		num = Node::NUM;
		++Node::NUM;
	}
	Node(bSudoku* bsudoku) 
	{
		this->bsudoku = bsudoku;
		possible = true;
		num = Node::NUM;
		++Node::NUM;
	}
	~Node()
	{
		delete bsudoku;
	}

        typedef vector<Node* > children_t;
        children_t children;

	bool possible;
	bSudoku* bsudoku;
	static int NUM;
	int num;
};


class Tree {
	std::stack<Node*> nstack;
	Node* root;
	
public:	
	Tree()
	{
		root = 0;
	}
	
	Tree(Node* node) 
	{
		root = node;
	}

	~Tree() 
	{
		rprunechilds(root);
		delete root;
	}

	Node* get_root()
	{
		return root;
	}
	
	size_t depth() { 
		return nstack.size(); 
	}

	void down(Node* node)
	{
		nstack.push(node);
	}

	Node* up()
	{
		if( nstack.empty() ) 
			return 0;
		else {
			Node* upnode = nstack.top();
			nstack.pop();
			return upnode;
		}		
	}
	
	void rprunechilds(Node* node)
	{
		if( !node->children.empty() ) {
			for(Node::children_t::iterator i = node->children.begin(); i != node->children.end(); ++i) {
				rprunechilds(*i);
				delete *i;
			}
			node->children.clear();
		}
	}
};

#endif
