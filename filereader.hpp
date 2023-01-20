//
//  filereader.hpp
//  CCN final
//
//  Created by Shivam Sharma on 20/01/23.
//


#ifndef filereading_hpp
#define filereading_hpp
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string read(string filename)
{
    string line, file_contents;
    ifstream file(filename);

    if (file.is_open())
    {
        while (getline(file, line))
        {
            file_contents += line + '\n';
        }
        file.close();
    }
    else cout << "Unable to open file";

//    cout << file_contents;

    return file_contents;
}
#endif /* filereading_hpp */
