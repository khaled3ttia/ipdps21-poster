#include <fstream>
#include <sstream>
#include <iostream>

void loadMTX(int *&nnzRowIdx, int *&nnzColIdx, float *&nnzVal, int* nnz, int *nrows, int *ncols,  std::string filename){

    //std::cout << "Loading file " << filename << "....." <<  std::endl;
	
	
	int numValues = 0;
	int currentLine = 0;
	int currentIndex = 0;
	
	
	std::string line;
	std::ifstream mtx_file (filename);
	

	if (mtx_file.is_open()){
		while (std::getline(mtx_file, line)) {
			std::istringstream iss(line);

			if (currentLine == 0) {

				// First line in MTX foramt is about the format
				// We are only dealing with COO format so just ignore it
				// but increment the count of lines
				currentLine++;
				continue;
			}else if (currentLine == 1) {

				// Second line contains number of rows, cols, and nnz
				if (!(iss >> *nrows >> *ncols >> *nnz)) {break;}
				
				//std::cout << "number of rows is: " << *nrows << std::endl;
				//std::cout << "number of cols is: " << *ncols << std::endl;
				//std::cout << "nnz is: " << *nnz << std::endl;	
				
				// dynamically allocate memory for COO arrays based on nnz				
				nnzColIdx = new int[*nnz];
				nnzRowIdx = new int[*nnz];
				nnzVal = new float[*nnz];

				// initialize all indices to -1
				
				//TOOD: = {-1} ?
				for (int i=0; i<*nnz; i++){
					nnzColIdx[i] = -1;
					nnzColIdx[i] = -1;
				}
	
				currentLine++;

			}else {

				
				// capture info from each line 
				// row index, column index, and nnz value

				int rowIndex, colIndex;
				float value;

				if (!(iss >> rowIndex >> colIndex >> value)) {break;}
				
				if (currentIndex < *nnz){
					
					nnzColIdx[currentIndex] = colIndex - 1;
					nnzRowIdx[currentIndex] = rowIndex - 1; 
					nnzVal[currentIndex] = value;
				
					currentIndex++;
				}
				currentLine++;

			}

		}


	}
	mtx_file.close();	
	
}

// a function to find the start location of each pattern in each row
void findStartLocations(float * row, size_t n, std::vector<int> &startLocations, int vectorWidth){
    
        int j = 0;

        while (j < n){
            
            if (row[j] == 0){
                j++;
                continue;
            }
            else {
                
                startLocations.push_back(j);
                j += vectorWidth;

            }

        }

}


