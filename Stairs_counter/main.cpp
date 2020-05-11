#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <opencv2/imgcodecs.hpp>
#include<iostream>
#include "img_processing.h"

using namespace std;
using namespace cv;

int main(){
	vector<vector<int>> mask = {
	{ 0 ,  0 ,  0},
	{ 0 , -1 ,  0},
	{ 0 ,  1 ,  0} };

	Mat img = imread("C:/Users/lilia/github/Stairs_Steps_Detection/Stairs/DB_2/s6.jpg", IMREAD_GRAYSCALE);
	//Si l'image ne charge pas on retourne une erreur
	if (!img.data)
	{
		cout << "Image not loaded";
		return -1;
	}
	cout << "Image loaded\n";
	//Création de l'objet
	img_processing i;

	//Affichage des informations sur les niveaux de gris de l'image 
	i.hist_info(img);

	//Affichage de l'image
	imshow("1_Image", img);

	//Application du filtre passe bas et affichage de l'image obtenue
	img = i.LPF(img);
	imshow("2_Image filtre passe bas", img);

	//Application d'un filtre qui extrait les contours horizontaux 
	img = i.LPF(img, mask);
	imshow("3_Extraction des contours", img);

	//Binarisation de l'image
	img = i.Binary(img, 50);
	imshow("4_Binarisation", img);

	//Closing : reduction du bruit sans détruire les éléments important 
	dilate(img, img, getStructuringElement(MORPH_RECT, Size(10, 1)));
	imshow("5_Dilatation 10*1", img);
	erode(img, img, getStructuringElement(MORPH_RECT, Size(75, 1)));
	imshow("6_Erosion 75*1", img);

	//Dilatation pour eviter la perte d'élément lors de l'érosion de l'opening
	dilate(img, img, getStructuringElement(MORPH_RECT, Size(15, 5)));
	imshow("7_Dilatation 15*5", img);

	//Opening
	erode(img, img, getStructuringElement(MORPH_RECT, Size(35, 1)));
	imshow("8_Erosion 35*1", img);
	dilate(img, img, getStructuringElement(MORPH_RECT, Size(35, 10)));
	imshow("9_Dilatation 35*10", img);

	//On passe ce qui est à 255 à 0 sur les premieres et dernières lignes 
	img = i.Del_rows(img);
	imshow("10_Deleted rows", img);
	
	//Comptage de marches
	i.count_mean(img);
	waitKey(0);
	return 0;
}