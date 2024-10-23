#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <filesystem>
#include <vector>
using namespace std;

namespace fs = filesystem;

//a function that returns the file type extention as a string
string getFileType(string file){
    string fileType = "";
    bool isType = false;
    for(const char &i : file){
        //when you find a dot, start writing into the fileType variable
        if(i == '.'){
            isType = true;
        }
        if(isType){
            fileType += i;
        }
    }
    return fileType;
}

//function that returns available picture options to draw
vector<string> getPictures(){
    //vector for saving options
    vector<string> options;
    //make a directory iterator to get directory info from
    auto directoryIterator = fs::directory_iterator("./");

    //iterate through directory files
    for(const auto &entry : directoryIterator){
        string data = entry.path().filename().string();

        //check if picture is .webp format
        if(getFileType(data) == ".jpg"){
            //push back just the file name without the ".jpg" file extention
            options.push_back(data.substr(0, data.size() - 4));
        }
    }
    return options;
}

//checks if the choice is available
bool checkImage(string image, vector<string> options){
    //cycle through options
    for(string i : options){
        if(image == i){
            return true;
        }
    }
    return false;
}


char getShade(int val){
    //convert the value into an appropriate character
    //best soo far je biu " ...++##@@"
    string values = " ...++##@@";
    int linearisedValue = val * (values.size() - 1) / 255;
    return values[linearisedValue];
}


void drawPicture(string image){
    //variables to save information into
    int width;
    int heigth;
    int channels;
    vector<int> pixels;

    //add extention to access and convert to const chat*
    image = image + ".jpg";
    const char* fileName = image.c_str();
    cout << fileName << endl;

    //load the picture
    unsigned char* img = stbi_load(fileName, &width, &heigth, &channels, 0);
    if(img){

        //get pixels and average the brightness
        for(int i = 0; i < width * heigth * channels; i+= channels){
            int pixel_brightness = (img[i] + img[i+1] + img[i+2]) / 3;
            pixels.push_back(pixel_brightness);
        }
        //close the image
        stbi_image_free(img);
    }
    else{
        cout << "Picture could not be loaded." << endl;
        cout << "Reason: " << endl << stbi_failure_reason() << endl;
        return;
    }
    for(int i = 0; i < heigth * width; i++){
        //check if you have to go into a new line
        if(i%width == 0){
            cout << endl;
        }
        //cycle through pixels and write the brightness character for them
        cout << getShade(pixels[i]) << " ";
    }
}

int main(){
    //get names of pictures and get 
    vector<string> pictureOptions = getPictures();

    //tell the user all the picture options to choose from
    cout << "--------------------" << endl;
    for(string i : pictureOptions){
        cout << i << endl;
    }
    cout << "--------------------" << endl;

    //get the input from the user
    string image;
    cout << "Choose an option from the list above: ";
    cin >> image;

    //check if the choice is valid
    bool correctChoice = checkImage(image, pictureOptions);
    if(correctChoice){
        drawPicture(image);
    }
    else{
        cout << "This is not a correct choice. " << endl;
    }

    return 0;
}