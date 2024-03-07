#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<filesystem>
#include"toojpeg.h"

std::vector<unsigned char>x;
void writeByte(unsigned char oneByte)
{
x.push_back(oneByte);
}
int main(){
    
    std::cout<<"Please enter the path of the bitmap file:-"<<std::endl;
    std::string path;
    while(true){
      std::getline(std::cin,path);
      if(not std::filesystem::exists(path) or not std::filesystem::is_regular_file(path) or std::filesystem::path(path).extension().string() not_eq ".bmp"){
          std::cerr<<"Invalid path or type of file! Please enter a correct path to a bitmap file:-"<<std::endl;
          path.clear();
          }
        else break;
      }
    std::fstream file(path,std::ios_base::in|std::ios_base::binary);
    if(not file.is_open()){
        std::cerr<<"Unable to open the file!"<<std::endl;
        return EXIT_FAILURE;
    }
    std::string name = std::filesystem::path(path).filename().string().substr(0,std::filesystem::path(path).filename().string().length()-4);
    name.append(".jpg");
    path = path.substr(0,path.length()-std::filesystem::path(path).filename().string().length());
    path.append(name);
    unsigned char info[54];
    file.read((char*)info,54);
    if(info[0] not_eq 'B' or info[1] not_eq 'M'){
      std::cerr<<"Invalid Bitmap file!"<<std::endl;
      return EXIT_FAILURE;
    }
    int width = *(int*)&info[18];
    int hight = *(int*)&info[22];
    std::size_t size = hight*width*3;
    int rowSize = (width*3+3)&~3;
    unsigned char* buff = new unsigned char[size];

    file.read(reinterpret_cast <char*>(buff),size);
    unsigned char*NewBuff = new unsigned char[size];
    file.close();
        for(int i = 0;i<hight;i++){
    for(int j = 0;j<width*3;j++){
        NewBuff[i*width*3+j] = buff[(hight-i-1)*rowSize+j];
    }
    }
    for(int i = 0;i<size;i+=3){
        std::swap(NewBuff[i],NewBuff[i+2]);
    }

   std::ofstream ffx(path,std::ios_base::binary);
    if(not TooJpeg::writeJpeg(writeByte,NewBuff,width,hight)){
        std::cerr<<"Unable to encode the file!"<<std::endl;
        return EXIT_FAILURE;
    }else {
      ffx.write(reinterpret_cast<char*>(x.data()),x.size());
      std::clog<<"Done!"<<std::endl;
      }
    
    ffx.close();
    delete [] buff;
    delete [] NewBuff;
    std::cout<<"Please type any key to close!"<<std::endl;
    std::cin.get();

    return EXIT_SUCCESS;
}
