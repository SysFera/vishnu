
#include <iostream>
#include <fstream>

using namespace std;

void ReplaceIf(const char* file, const string& first_line, 
               const string& second_line, const string& output_line){

   int length;
   char *buffer;
   size_t pos;
   size_t pos_end;
   size_t pos2;
   size_t pos_end2;
   size_t output_size;

   ifstream is(file);

   if(!is.is_open()) {
     cout << "Can't open " << file << endl;
     return;
   }

   is.seekg(0, ios::end);
   length = is.tellg();
   is.seekg(0, ios::beg);

   buffer = new char[length];
   is.read(buffer, length);

   string str_buffer(buffer);

   pos = str_buffer.find(first_line);
   if(pos!=string::npos) {
     pos_end = pos+first_line.size();
     str_buffer.erase(pos, pos_end-pos);
 
     output_size = length-(pos_end-pos);
    
     pos2 = str_buffer.find(second_line);
     if(pos2!=string::npos) {
        pos_end2 = pos2+second_line.size();
        str_buffer.erase(pos2, pos_end2-pos2);
        output_size -= (pos_end2-pos2);
     }

    string line_to_replace = "<book>\n";
    pos = str_buffer.find(line_to_replace);
    if(pos!=string::npos) {
      str_buffer.replace(pos, line_to_replace.size(), output_line);  
      output_size  += (output_line.size()-line_to_replace.size());
    }

    is.close();

    ofstream os(file);
    os.write(str_buffer.c_str(), output_size);
    os.close();

   } else is.close();

}


int main(int argc, char* argv[]) {

  if(argc < 2) {
    cout << "Usage : " << argv[0] << "  <fileToTransform>" << endl;
  }

  string first_line = "<Docbook:DocumentRoot xmlns:Docbook=\"http://docbook.org/ns/docbook\">\n";
  string second_line = "</Docbook:DocumentRoot>\n";
  string output_line = "<book xmlns=\"http://docbook.org/ns/docbook\">\n";
  ReplaceIf(argv[1], first_line, second_line, output_line);


 return 0;
}
