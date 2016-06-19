/*================================================================
*   Copyright (C) 2016 All rights reserved.
*   
*   filename     :mnist_read.cpp
*   author       :qinlibin
*   create date  :2016/06/19
*   mail         :qin_libin@foxmail.com
*
================================================================*/
//#include "mnist_read.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
using namespace cv;
using namespace std;

int str2int(string src)
{
    int num;
    istringstream iss(src);
    iss>>num;
    return num;
}

string int2str(int n)
{
    ostringstream oss;
    string str;
    oss<<n;
    str = oss.str();
    return str;
}

void swap_buff(char buff[])
{
    char tmp;
    tmp = buff[0];
    buff[0] = buff[3];
    buff[3] = tmp;
    tmp = buff[1];
    buff[1] = buff[2];
    buff[2] = tmp;
}

int read_and_save_train_img(int mode)
{
    ifstream ifs;
    int ret = 0;
    ifs.open("train-images-idx3-ubyte",ios::in | ios::binary);
    if(!ifs)
    {
        cout << "training image file open error!" <<endl;
        ret = -1;
        return ret;
    }
    string w_dir = "train_image/";
    string dst_name;
    if(access("train_image",F_OK))
    {
        system("mkdir train_image");
    }
    //read magic number and check
    int magic_num = 0, num_img = 0,rows = 0, cols = 0;
    char magic_num_str[4], num_img_str[4];
    char row_str[4],col_str[4];
    ifs.read(magic_num_str,4);
    swap_buff(magic_num_str);
    memcpy(&magic_num,magic_num_str,sizeof(magic_num));
    if(magic_num != 2051)
    {
        cout<< "magic number of training image file check failed!"<<endl;
        ret = -1;
        ifs.close();
        return ret;
    }
    //read image number
    ifs.read(num_img_str,4);
    swap_buff(num_img_str);
    memcpy(&num_img,num_img_str,sizeof(num_img));
//    cout<<  num_img <<endl;
    //read image size;
    ifs.read(row_str,4);
    ifs.read(col_str,4);
    swap_buff(row_str);
    swap_buff(col_str);
    memcpy(&rows,row_str,sizeof(rows));
    memcpy(&cols,col_str,sizeof(cols));
//    cout<<rows<<","<<cols<<endl;
    //read image datas
    Mat image = Mat(rows,cols,CV_8UC1);
    cout<<"starting processing train images"<<endl;
    for(int i = 0; i < num_img; i++)
    {
        ifs.read((char *)image.data,rows * cols);
        if(mode)
        {
            imshow("1",image);
            int k = waitKey(20);
            if(k == 'b')
            {
                cout<<"break out"<<endl;
                break;
            }
        }

        dst_name = w_dir+"train_" + int2str(i) + ".jpg";
        imwrite(dst_name,image);
    }
    ifs.close();
    return ret;
}


int read_and_save_test_img(int mode)
{
    ifstream ifs;
    int ret = 0;
    ifs.open("t10k-images-idx3-ubyte",ios::in | ios::binary);
    if(!ifs)
    {
        cout << "testing image file open error!" <<endl;
        ret = -1;
        return ret;
    }
    string w_dir = "test_image/";
    string dst_name;
    if(access("test_image",F_OK))
    {
        system("mkdir test_image");
    }
    //read magic number and check
    int magic_num = 0, num_img = 0,rows = 0, cols = 0;
    char magic_num_str[4], num_img_str[4];
    char row_str[4],col_str[4];
    ifs.read(magic_num_str,4);
    swap_buff(magic_num_str);
    memcpy(&magic_num,magic_num_str,sizeof(magic_num));
    if(magic_num != 2051)
    {
        cout<< "magic number of testing image file check failed!"<<endl;
        ret = -1;
        ifs.close();
        return ret;
    }
    //read image number
    ifs.read(num_img_str,4);
    swap_buff(num_img_str);
    memcpy(&num_img,num_img_str,sizeof(num_img));
//    cout<<  num_img <<endl;
    //read image size;
    ifs.read(row_str,4);
    ifs.read(col_str,4);
    swap_buff(row_str);
    swap_buff(col_str);
    memcpy(&rows,row_str,sizeof(rows));
    memcpy(&cols,col_str,sizeof(cols));
//    cout<<rows<<","<<cols<<endl;
    //read image datas
    Mat image = Mat(rows,cols,CV_8UC1);
    cout<<"starting processing test images"<<endl;
    for(int i = 0; i < num_img; i++)
    {
        ifs.read((char *)image.data,rows * cols);
        if(mode)
        {
            imshow("1",image);
            int k = waitKey(20);
            if(k == 'b')
            {
                cout<<"break out"<<endl;
                break;
            }
        }
        dst_name = w_dir+"test_" + int2str(i) + ".jpg";
        imwrite(dst_name,image);
    }
    ifs.close();
    return ret;
}

void verify(string vfile)
{
    ifstream ifs;
    string line;
    string filename;
    int label;
    ifs.open(vfile.c_str());
    cout<<"start verify "<<vfile<<endl;
    if(!ifs)
    {
        cout<<"No "<<vfile<<"  in current directory!"<<endl;
        return;
    }
    while(!ifs.eof())
    {
        getline(ifs,line);
        istringstream iss(line);
        iss>>filename>>label;
        cout<<filename<<" "<<label<<endl;
        Mat img = imread(filename.c_str());
        imshow("img",img);
        if(waitKey(0) == 'b')
            break;
    }

    ifs.close();
    return;
}


int read_and_save_train_label()
{
    ifstream ifs;
    ofstream ofs;
    unsigned char label;
    string img_prefix = "train_image/train_";
    int ret = 0;
    ifs.open("train-labels-idx1-ubyte",ios::in | ios::binary);
    if(!ifs)
    {
        cout << "training label file open error!" <<endl;
        ret = -1;
        return ret;
    }
    string dst_name = "train_label.txt";
    ofs.open(dst_name.c_str(),ios::out);
    //read magic number and check
    int magic_num = 0, num_img = 0;
    char magic_num_str[4], num_img_str[4];
    ifs.read(magic_num_str,4);
    swap_buff(magic_num_str);
    memcpy(&magic_num,magic_num_str,sizeof(magic_num));
    if(magic_num != 2049)
    {
        cout<< "magic number of training label file check failed!"<<endl;
        ret = -1;
        ifs.close();
        return ret;
    }
    //read image number
    ifs.read(num_img_str,4);
    swap_buff(num_img_str);
    memcpy(&num_img,num_img_str,sizeof(num_img));
    for(int i = 0; i < num_img; i++)
    {
        ifs.read((char *)(&label),sizeof(char));
        string img_name = img_prefix+int2str(i)+".jpg";
        memcpy(&label,&label,sizeof(label));
        ofs<<img_name<<" "<<int2str(label)<<endl;

    }
    ifs.close();
    ofs.close();
    return ret;
}


int read_and_save_test_label()
{
    ifstream ifs;
    ofstream ofs;
    unsigned char label;
    string img_prefix = "test_image/test_";
    int ret = 0;
    ifs.open("t10k-labels-idx1-ubyte",ios::in | ios::binary);
    if(!ifs)
    {
        cout << "testing label file open error!" <<endl;
        ret = -1;
        return ret;
    }
    string dst_name = "test_label.txt";
    ofs.open(dst_name.c_str(),ios::out);
    //read magic number and check
    int magic_num = 0, num_img = 0;
    char magic_num_str[4], num_img_str[4];
    ifs.read(magic_num_str,4);
    swap_buff(magic_num_str);
    memcpy(&magic_num,magic_num_str,sizeof(magic_num));
    if(magic_num != 2049)
    {
        cout<< "magic number of testing label file check failed!"<<endl;
        ret = -1;
        ifs.close();
        return ret;
    }
    //read image number
    ifs.read(num_img_str,4);
    swap_buff(num_img_str);
    memcpy(&num_img,num_img_str,sizeof(num_img));
    for(int i = 0; i < num_img; i++)
    {
        ifs.read((char *)(&label),sizeof(char));
        string img_name = img_prefix+int2str(i)+".jpg";
        memcpy(&label,&label,sizeof(label));
        ofs<<img_name<<" "<<int2str(label)<<endl;

    }
    ifs.close();
    ofs.close();
    return ret;
}
int read_and_save_all(int mode)
{
    if(!read_and_save_train_img(mode) &&
    !read_and_save_test_img(mode) &&
    !read_and_save_train_label() &&
    !read_and_save_test_label() )
        return 0;
    else
        return -1;
}


void download_data()
{
    if(access("train-images-idx3-ubyte",F_OK) && access("train-images-idx3-ubyte.gz",F_OK))
    {
        system("wget http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz");
    }
    if(!access("train-images-idx3-ubyte.gz",F_OK))
    {
        system("gzip -d train-images-idx3-ubyte.gz");
    }

    if(access("train-labels-idx1-ubyte",F_OK) && access("train-labels-idx1-ubyte.gz",F_OK))
    {
        system("wget http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz");
    }
    if(!access("train-labels-idx1-ubyte.gz",F_OK))
    {
        system("gzip -d train-labels-idx1-ubyte.gz");
    }

    if(access("t10k-labels-idx1-ubyte",F_OK) && access("t10k-labels-idx1-ubyte.gz",F_OK))
    {
        system("wget http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz");
    }
    if(!access("t10k-labels-idx1-ubyte.gz",F_OK))
    {
        system("gzip -d t10k-labels-idx1-ubyte.gz");
    }

    if(access("t10k-images-idx3-ubyte",F_OK) && access("t10k-images-idx3-ubyte.gz",F_OK))
    {
        system("wget http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz");
    }
    if(!access("t10k-images-idx3-ubyte.gz",F_OK))
    {
        system("gzip -d t10k-images-idx3-ubyte.gz");
    }
}

int main(int argc, char *argv[])
{
    int visible = 0,validate = 0;
    string arg1,arg2;
    if(argc >= 2)
    {
        arg1 = argv[1];
    }
    if(argc == 3)
    {
        arg2 = argv[2];
    }
    if(argc > 3)
    {
        cout<<"too much params"<<endl;
        return -1;
    }
    if(arg1 == "-t" || arg2 == "-t")
    {
        validate = 1;
    }
    if(arg1 == "-v" || arg2 == "-v")
    {
        visible = 1;
    }
    if(argc >= 2 && arg1 != "-t" && arg1 != "-v")
    {
        cout<<"param not recognized"<<endl;
        return -1;
    }
    if(argc == 3 && arg2 != "-t" && arg2 != "-v")
    {
        cout<<"param not recognized"<<endl;
        return -1;
    }

    download_data();
    read_and_save_all(visible);
    if(validate)
    {
        verify("train_label.txt");
        verify("test_label.txt");
    }
    return 0;
}
