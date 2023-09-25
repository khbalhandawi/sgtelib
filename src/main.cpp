#include "sgtelib.hpp"
#include "Surrogate_Factory.hpp"
#include "Surrogate_Utils.hpp"
#include <fstream>
#include <string>
using namespace SGTELIB;


void count_rows(std::ifstream & file, int & rows, int & cols) {
    // First, count the number of rows and columns in the CSV file.
    std::string line;

    while (std::getline(file, line)) {
        ++rows;
        std::istringstream lineStream(line);
        std::string cell;
        int currentCols = 0;
        while (std::getline(lineStream, cell, ',')) {
            ++currentCols;
        }
        if (currentCols > cols) {
            cols = currentCols;
        }
    }
}


void copy_array(std::ifstream & file, int & rows, int & cols, double **doubleArray ) {

    std::string line;

    // Reset the file stream and read the data into the double** array.
    file.clear();
    file.seekg(0);
    int rowIdx = 0;

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string cell;
        int colIdx = 0;

        while (std::getline(lineStream, cell, ',')) {
            doubleArray[rowIdx][colIdx] = std::stod(cell);
            ++colIdx;
        }

        // If the row had fewer columns than the maximum, fill the rest with zeros.
        for (; colIdx < cols; ++colIdx) {
            doubleArray[rowIdx][colIdx] = 0.0;
        }

        ++rowIdx;
    }
}

double ** allocate_array(int & rows, int & cols) {
    double** doubleArray = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        doubleArray[i] = new double[cols];
    }
    return doubleArray;
}


void free_array(int & rows, int & cols, double **doubleArray ) {
    for (int i = 0; i < rows; ++i) {
        delete[] doubleArray[i];
    }
    delete[] doubleArray;
}

/*--------------------------------------*/
/*           main function              */
/*--------------------------------------*/
int main ( int argc , char ** argv ) {

    int rows;
    int cols;
    SGTELIB::Matrix X,Z,XX,ZZ;
    std::ifstream file;


    rows = 0;
    cols = 0;
    file.open("X.txt");

    count_rows(file, rows, cols);
    double** doubleX = allocate_array(rows,cols); // create new double array
    copy_array(file, rows, cols, doubleX);
    X = SGTELIB::Matrix("X",rows,cols,doubleX);
    
    file.close();
    
    rows = 0;
    cols = 0;
    file.open("Z.txt");

    count_rows(file, rows, cols);
    double** doubleZ = allocate_array(rows,cols); // create new double array
    copy_array(file, rows, cols, doubleZ);
    Z = SGTELIB::Matrix("Z",rows,cols,doubleZ);

    file.close();

    rows = 0;
    cols = 0;
    file.open("XX.txt");

    count_rows(file, rows, cols);
    double** doubleXX = allocate_array(rows,cols); // create new double array
    copy_array(file, rows, cols, doubleXX);
    XX = SGTELIB::Matrix("XX",rows,cols,doubleXX);

    file.close();

    std::string model = "TYPE PRS DEGREE 2";    

    SGTELIB::TrainingSet TS(X,Z);
    SGTELIB::Surrogate * S = Surrogate_Factory(TS,model);
    S->build();
    ZZ = SGTELIB::Matrix("ZZ",XX.get_nb_rows(),Z.get_nb_cols());
    S->predict(XX,&ZZ);
    ZZ.set_name("ZZ");
    ZZ.display(std::cout);

    // free_array(rows, cols, doubleX);
}