#include "sgtelib.hpp"
#include "Surrogate_Factory.hpp"
#include "Surrogate_Utils.hpp"
#include <fstream>
#include <string>
using namespace SGTELIB;

/*--------------------------------------*/
/*           main function              */
/*--------------------------------------*/
int main ( int argc , char ** argv ) {

    std::string model = "TYPE PRS DEGREE 2";
    SGTELIB::Matrix X,Z,XX,ZZ;
    
    X = SGTELIB::Matrix("X.txt");
    Z = SGTELIB::Matrix("X.txt");
    XX = SGTELIB::Matrix("X.txt");

    SGTELIB::TrainingSet TS(X,Z);
    SGTELIB::Surrogate * S = Surrogate_Factory(TS,model);
    S->build();
    ZZ = SGTELIB::Matrix("ZZ",XX.get_nb_rows(),Z.get_nb_cols());
    S->predict(XX,&ZZ);
    ZZ.set_name("ZZ");
    ZZ.display(std::cout);

}