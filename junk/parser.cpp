#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include <QTextStream>
#include <QString>
#include <QFile>

using namespace std;

int main ( int argc, char * argv[] ) {

  if (argc < 2 ) {
    cerr << "Need a filename" << endl;
    exit ( EXIT_FAILURE );
  }
  string filename = argv[1];
  QString qfilename = argv[1];

  QFile f(qfilename);
  if (! f.open( QIODevice::ReadOnly | QIODevice::Text)) {
    cerr << "Can not open file" << qfilename.toStdString() << endl;
    exit ( EXIT_FAILURE );
  }    
  
  cout << "Dumping contents of filename: " << qfilename.toStdString() << endl;

  char c;
  //  while ( f.get(c) ) {
  QTextStream in(&f);
  while ( !in.atEnd() ) {
    QString line = in.readLine ();
    cout << line.toStdString() << endl;
  }

}
