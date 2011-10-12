#include "Sudoku.hh"
#include <cstdlib>

using namespace std;
string file;

void usage(char *argv[]) {
	cout << argv[0] << " [filename]" << endl;
}
	
int main (int argc, char *argv[])
{
	Sudoku s;
	string file;
	if ( argc != 2 ) {
		usage(argv);
		return EXIT_FAILURE;
	} else {
		file = argv[1];
	}
	if( ! s.read_from_file(file) ) {
		cerr << "file is empty" << endl;
		return EXIT_FAILURE;
	}

	if ( ! file.empty() ) {
	}
	s.disp_data();
	s.calc_used();
	cout << endl <<  "Using depth-first search\n" << endl ;
	Tree tree(new Node(s.Clone()));
	Node* curr = tree.get_root(); 
	int maxdepth=0;
	int hypnum=0;
	while(1) {
		DLOG(cout << "Depth :" << tree.depth() << endl);
		if ( tree.depth() > maxdepth )
			maxdepth = tree.depth();
		//curr->bsudoku->disp_used();
		while( curr->bsudoku->elimination() ) {
			if( curr->bsudoku->is_impossible() ) {
				DLOG(cout << "** Impossible **\n" << endl);
				curr->possible = false;	
				break;
			}
		}
		if ( curr->bsudoku->is_solved() ) {
			cout << "Sudoku solved" << endl;
			cout << "maxdepth: " << maxdepth << endl << "hypnum: " << hypnum << endl;
			Sudoku solved(curr->bsudoku);
			s.disp_data();
			solved.disp_result();
			return EXIT_SUCCESS;

		} else if ( bSudoku* hypothesis = curr->bsudoku->new_hypothesis() ) {
			DLOG(cout << "New hypothesis on node number: " << curr->num << endl);
			//hypothesis->disp_used();
			++hypnum;
			Node* child = new Node(hypothesis);
			curr->children.push_back(child);
			tree.down(curr);
			curr = child;
		} else {
			DLOG(cout << "Not solved and hypothesis depleted\n" << endl);
			curr->possible = false;
			tree.rprunechilds(curr);
			if ( ! ( curr = tree.up() ) ) {
				cout << "Sudoku impossible";
				cout << "maxdepth: " << maxdepth << endl << "hypnum: " << hypnum << endl;
				return EXIT_FAILURE;
			}	
		}
	}
}
