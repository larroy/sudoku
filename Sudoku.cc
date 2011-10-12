#include "Sudoku.hh"
#include <cstdlib>
int Node::NUM = 0;

bSudoku::bSudoku()
{
	used = new bitset<SIZE>*[SIZE];
	for (short i = 0; i < SIZE; ++i) {
		used[i] = new bitset<SIZE>[SIZE];
	}

	flags = new bitset<FLAGSSIZE>* [SIZE];
	for (short i = 0; i < SIZE; ++i) {
		flags[i] = new bitset<FLAGSSIZE>[SIZE];
	}
	
	t_hyp = new bitset<SIZE>* [SIZE];
	for (short i = 0; i < SIZE; ++i) {
		t_hyp[i] = new bitset<SIZE>[SIZE];
	}
}	

bSudoku::~bSudoku()
{
	for (short i = 0; i < SIZE; ++i) {
		delete[] used[i];
	}
	delete[] used;

	for (short i = 0; i < SIZE; ++i) {
		delete[] flags[i];
	}
	delete[] flags;

	for (short i = 0; i < SIZE; ++i) {
		delete[] t_hyp[i];
	}
	delete[] t_hyp;

}	

bSudoku* bSudoku::Clone()
{
	bSudoku* cloned = new bSudoku();
	for (short j = 0; j < SIZE; ++j) 
		for (short i = 0; i < SIZE; ++i) {
			cloned->used[j][i] = this->used[j][i];
			cloned->flags[j][i] = this->flags[j][i];	
			cloned->t_hyp[j][i] = this->t_hyp[j][i];	
		}		
	cloned->gflags = this->gflags;
	return cloned;
}

bSudoku* bSudoku::new_hypothesis()
{
	for (short j = 0; j < SIZE; ++j) 
		for (short i = 0; i < SIZE; ++i) 
			t_hyp[j][i] = t_hyp[j][i] | ~ used[j][i];

	// Caution: three levels of flags to speed up things ahead, tested in order
	if( gflags.test(no_hyp_left) )
		return 0;
	else {
		short min_i=-1, min_j=-1;
		bool found=false;
		size_t size;
		int	min_size=SIZE;
		// Those with lesser number of possible hypothesis are believed to converge first, ie. spawn a smaller tree at least
		for (short j = 0; j < SIZE; ++j)
			for (short i = 0; i < SIZE; ++i)
				if ( ! flags[j][i].test(done) && ! flags[j][i].test(hyp_tried) && (size = used[j][i].count()) >= 2 && size < min_size) {
					found = true;
					min_size = size;
					min_i = i;
					min_j = j;
				}		
		if ( ! found ) {
			gflags.set(no_hyp_left);
			return 0;
			
		} else {  // Find a bit to make a hypothesis
			for(short i=0; i <  SIZE; ++i) {
				if ( ! t_hyp[min_j][min_i].test(i) && used[min_j][min_i].test(i)   ) {
					DLOG(cout << "hypothesis on: (" << min_j << "," << min_i << "," << i << ")" << endl);
					DLOG(disp_used());
					DLOG(disp_t_hyp());
					t_hyp[min_j][min_i].set(i);
					/*if ( ( t_hyp[min_j][min_i] ^ used[min_j][min_i] ) == 0 ) 
						flags[min_j][min_i].set(hyp_tried);*/
					bSudoku* hyp = Clone();
					hyp->used[min_j][min_i].reset(i);
					return hyp; // Yes, now we found an hypothesis
				}
			}
		}
	}
	return 0;
}

void bSudoku::disp_t_hyp ()
{
	cout << "t_hyp: " << endl;
	for (short j = 0; j < SIZE; ++j) {
		if( j > 0 && j % 3 == 0 )
			cout << endl;
		for (short i = 0; i < SIZE; ++i) {
			if( i > 0 && ((i % 3) == 0) )
				cout << " ";
			cout << t_hyp[j][i] << " ";
		}
		cout << endl;
	}
}

void bSudoku::disp_used ()
{
	cout << "used: " << endl;
	for (short j = 0; j < SIZE; ++j) {
		if( j > 0 && ((j % 3) == 0) )
			cout << endl;
		for (short i = 0; i < SIZE; ++i) {
			if( i > 0 && i % 3 == 0 )
				cout << " ";
			cout << used[j][i] << " ";
		}
		cout << endl;
	}
}

void bSudoku::disp_flags ()
{
	cout << "flags: " << endl;
	for (short j = 0; j < SIZE; ++j) {
		for (short i = 0; i < SIZE; ++i) {
			cout << flags[j][i] << " ";
		}
		cout << endl;
	}
}


bool bSudoku::is_impossible() 
{
	for (short j = 0; j < SIZE; ++j) {
		for (short i = 0; i < SIZE; ++i) {
			if ( used[j][i].count() == 0 ) {
				DLOG(cout << "impossible elmt on: (" << j << "," << i << ")\n");
				return true;
			}
		}
	}	
	return false;
}

bool bSudoku::is_solved()
{
	for (short j = 0; j < SIZE; ++j) {
		for (short i = 0; i < SIZE; ++i) {
			if ( ! flags[j][i].test(done) )
				return false;
		}
	}	
	return true;
}

bool bSudoku::elimination()
{
	bool found=false;
	for (short j = 0; j < SIZE; ++j) {
		for (short i = 0; i < SIZE; ++i) {
			if ( ! flags[j][i].test(done) &&  used[j][i].count() == 1 ) {
				flags[j][i].set(done);
				found=true;

				DLOG(cout << "definitive elmt on: (" << j << "," << i << ")\n");
				// It's definitive, sweep column i and row j
				DLOG(cout << "sweep row: " << j << " with " << used[j][i] << endl);	
				for(short k=0; k < SIZE; k++) {
					if ( k != i ) {
						DLOG(cout << j << "," << k << " " << used[j][k] << " before" << endl);
						used[j][k] = used[j][k] & ~ used[j][i];
						DLOG(cout << j << "," << k << " " << used[j][k] << " after" << endl);
					}	
				}

				DLOG(cout << "sweep column: " << i << " with " << used[j][i] << endl);	
				for(short k=0; k < SIZE; k++) {
					if ( k != j) {
						DLOG(cout << k << "," << i << " " << used[k][i] << " before" << endl);
						used[k][i] = used[k][i] &~ used[j][i];  
						DLOG(cout << k << "," << i << " " << used[k][i] << " after" << endl);
					}	
				}
				// Sweep its quadrant
				short row		= (j/CUADRANT)*CUADRANT; // integer division
				short row_lim	= row+CUADRANT;
				short col		= (i/CUADRANT)*CUADRANT; // integer division
				short col_lim	= col+CUADRANT;
				DLOG(cout << "sweep quadrant: ("  << row << "," << col << ") to (" << row_lim << "," << col_lim << ") with " << used[j][i] << endl);	
				
				for( ; row < row_lim; row++) {
					for(col = (i/CUADRANT)*CUADRANT ; col < col_lim; col++) {
						if ( ! (row == j &&  col == i) ) {
							DLOG(cout << row << "," << col << " " << used[row][col] << "before" << endl);
							used[row][col] = used[row][col] &~ used[j][i]; 
							DLOG(cout << row << "," << col << " " << used[row][col] << "after" <<  endl);
						} else {
							DLOG(cout << "skip self (" << row << "," << col << ")" << endl);
						}
					}
				}
			}
		}
	}	
	DLOG(cout << endl << " elimination found: " << found << endl);
	return found;
}



Sudoku::Sudoku() : bSudoku()
{
	data = new short *[SIZE];
	for (short i = 0; i < SIZE; ++i) {
		data[i] = new short[SIZE];
	}
	result = new short *[SIZE];
	for (short i = 0; i < SIZE; ++i) {
		result[i] = new short[SIZE];
	}
}

Sudoku::Sudoku(bSudoku* bsudoku) : bSudoku()
{
	data = new short *[SIZE];
	for (short i = 0; i < SIZE; ++i) {
		data[i] = new short[SIZE];
	}
	result = new short *[SIZE];
	for (short i = 0; i < SIZE; ++i) {
		result[i] = new short[SIZE];
	}
	for (short j = 0; j < SIZE; ++j) 
		for (short i = 0; i < SIZE; ++i) {
			data[j][i] = 0;
			result[j][i] = 0;
			this->used[j][i] = bsudoku->used[j][i];
			this->flags[j][i] = bsudoku->flags[j][i];	
			this->t_hyp[j][i] = bsudoku->t_hyp[j][i];	
		}		
	this->gflags = bsudoku->gflags;
}

Sudoku::~Sudoku ()
{
	for (short i = 0; i < SIZE; ++i) {
		delete[] data[i];
	}
	delete[] data;

	for (short i = 0; i < SIZE; ++i) {
		delete[] result[i];
	}
	delete[] result;
}


void Sudoku::calc_used()
{
	for (short j = 0; j < SIZE; ++j) {
		for (short i = 0; i < SIZE; ++i) {
			if ( data[j][i]  != 0 ) {
				used[j][i].flip(data[j][i]-1);
			} else {
				used[j][i].set();
			}
		}
	}	
}


void Sudoku::disp_data ()
{
	cout << "data: " << endl;
	for (short j = 0; j < SIZE; ++j) {
		for (short i = 0; i < SIZE; ++i) {
			cout << data[j][i] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Sudoku::disp_result ()
{
	cout << "result: " << endl;
	for (short j = 0; j < SIZE; ++j) {
		for (short i = 0; i < SIZE; ++i) {
			if( flags[j][i].test(done) ) {
				bool found=false;
				for(short k=0; k < SIZE; k++) {
					if ( used[j][i].test(k) ) {
						found=true;
						result[j][i] = k+1;
					}	
				}
				if ( found == false ) 
					result[j][i] = 0;
			}	
			cout << result[j][i] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Sudoku::initialize ()
{
	for (short j = 0; j < SIZE; ++j) {
		for (short i = 0; i < SIZE; ++i) {
			cout << "Fila " << (j + 1) << ", columna " << (i + 1) << ": ";
			cin >> data[j][i];
		}
	}
}

bool Sudoku::read_from_file (const string& file)
{
	ifstream fin(file.c_str());
	if ( ! fin ) {
		cerr << "can't open: " << file << endl;
		return false;
	}
	for(int j=0;j<SIZE ; ++j) {
		for(int i=0; i<SIZE  && !fin.eof() ; ++i) {
			string s;
			fin >> s;
			if( !s.empty() && s.compare(0,1,"0",1) >=0  && s.compare(0,1,"9",1) <=0 ) {
				data[j][i] = std::atoi(s.c_str());
			} else {
				data[j][i] = 0;
			}

			cout << "read: (" << j << "," << i << ") " << flush << data[j][i] << endl;
		}	
	}
	return true;
}
