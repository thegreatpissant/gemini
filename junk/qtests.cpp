/* James A. Feister jfeister@udel.edu
 *
 * Types: are First letter Capitalized following words lowercase
 *   seperation by underscores.
 *
 * declared functions and variables are lowercase
 *  seperation by underscores.
 */

#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <QFile>

#include <string>
#include <vector>
#include <iostream>

/*
 * load_file
 * success: return std::string of file contents
 * fail: return an empty std::string
 * TODO: should throw an error
 */
std::vector<std::string> load_file ( QString filename )
{
    QFile q_file(filename);
    if (!q_file.open( QIODevice::ReadOnly | QIODevice::Text) )
    {
        return std::vector< std::string >();   //  Return empty string
    }

    std::vector<std::string> source_code;
    QTextStream in_stream (&q_file);
    while (!in_stream.atEnd())
    {
        source_code.push_back ( in_stream.readLine().toStdString() );
    }
    return source_code;
}

std::vector <std::size_t> verify_source (std::vector<std::string> source_code)
{
    const std::string delimiters (" \t!");
    std::vector <std::size_t> line_errors;
    bool found_error {false};
    bool main_found {false};

    for ( std::size_t i = 0; i < source_code.size(); i++ )
    {
        std::string::size_type idx_inst_beg, idx_op_end;
        std::string opcode;
        std::string current_line = source_code[i];
        idx_inst_beg = current_line.find_first_not_of(" \t");
        if (current_line[idx_inst_beg] == '!' || idx_inst_beg == std::string::npos)
            continue;
        idx_op_end = current_line.substr(idx_inst_beg).find_first_of(" :\n");

        opcode = current_line.substr(idx_inst_beg, idx_op_end);

        if (opcode == "lda")
        {
        }
        else if (opcode == "sta")
        {
        }
        else if (opcode == "add")
        {
        }
        else if (opcode == "sub")
        {
        }
        else if (opcode == "or")
        {
        }
        else if (opcode == "and")
        {
        }
        else if (opcode == "nota")
        {
        }
        else if (opcode == "nop")
        {
        }
        else if (opcode == "bl")
        {
        }
        else if (opcode == "ba")
        {
        }
        else if (current_line[idx_inst_beg + idx_op_end] != ':')
        {
            std::cout << "!= :" << std::endl;
            line_errors.push_back(i);
            continue;
        }
        else if (current_line[idx_inst_beg + idx_op_end] == ':')
        {
            std::cout << "lable: " << opcode << std::endl;
            if (opcode == "main")
                main_found = true;
            continue;
        }
        std::cout << "opcode: " << opcode << std::endl;
    }

    if (!main_found)
        line_errors.push_back(-1);

    if ( ! line_errors.empty() )
        return line_errors;
    return std::vector <std::size_t> ();
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream qout (stdout);
    QTextStream qerr (stderr);

    if ( argc < 2 )
    {
        qerr << "Need a filename" << endl;
        exit (EXIT_FAILURE);
    }

    //  Load source code
    std::vector<std::string> source_code = load_file( QString (argv[1]) );
    if ( source_code.empty() )
    {
        qerr << "file did not load properly" << endl;
        exit (EXIT_FAILURE);
    }

    // Verify the source code
    std::vector <std::size_t> errors = verify_source (source_code);
    if (!errors.empty())
    {
        for (auto a : errors)
        {
            if ( a == -1)
            {
                qerr << "Did not find a lable main" << endl;
                continue;
            }
            qerr << "Found error at line: " << a << endl;
            qerr << "\t" << QString::fromStdString(source_code[a]) << endl;
        }
        exit (EXIT_FAILURE);
    }

    qout << "Source file verified!!" << endl;

}
