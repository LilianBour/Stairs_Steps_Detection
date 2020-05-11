#include "img_processing.h"

vector<float> img_processing::hist_info(Mat img) {
	float mean=0;
	int counter = 0;
	int max = 0;
	int min = 256;
	pair<int,int> count_mode = make_pair(-1,-1);
	vector<int> grey_values(256, 0);

	//Passage par tous les px pour calculer la moyenne, max, min and mode 
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			counter = counter + 1;
			mean = mean + (int)img.at<uchar>(i, j);
			if ((int)img.at<uchar>(i, j) < min) {
				min = (int)img.at<uchar>(i, j);
			}
			if ((int)img.at<uchar>(i, j) > max) {
				max = (int)img.at<uchar>(i, j);
			}
			grey_values[(int)img.at<uchar>(i, j)] = grey_values[(int)img.at<uchar>(i, j)] + 1;
			if (grey_values[(int)img.at<uchar>(i, j)] > count_mode.first) {
				count_mode.first = grey_values[(int)img.at<uchar>(i, j)];
				count_mode.second = (int)img.at<uchar>(i, j);
			}

		}
	}

	mean = mean / counter;
	//Calcule de l'écart type
	float sxm;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			sxm = ((int)img.at<uchar>(i, j) * mean) * ((int)img.at<uchar>(i, j) * mean);
		}
	}
	float sd = sqrt(sxm / (counter - 1));

	//Affichage des données
	cout << "Mean : " << mean << "\nSD : " << sd << "\nMode : " << count_mode.second << "\nMax : " << max << "\nMin : " << min << endl;
	//Renvoie des données
	return { (float)max,(float)min,(float)count_mode.second ,mean,sd };
}

Mat img_processing::Binary(Mat img, int threshold) {
	//Creation d'une image binary de la meme taille que l'image passée en paramètre
	Mat binary(img.rows, img.cols, CV_8UC1, Scalar(70));

	//Pour chaques pixels si c'est en dessous du seuil on met a 0 et si c'est au dessus ou égale au seuil on met a 255
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if ((int)img.at<uchar>(i, j) > threshold or (int)img.at<uchar>(i, j) == threshold ) {
				binary.at<uchar>(i, j) = 255;
			}
			if ((int)img.at<uchar>(i, j) < threshold) {
				binary.at<uchar>(i, j) = 0;
			}
		}
	}
	//Renvoie l'image binaire
	return binary;
}

Mat img_processing::Del_rows(Mat img) {
	//Creation d'une image binary de la meme taille que l'image passée en paramètre
	Mat deleted(img.rows, img.cols, CV_8UC1, Scalar(70));

	//On met les 5 premier et derniere rangee a 0 
	for (int i = 0; i < deleted.rows; i++) {
		for (int j = 0; j < deleted.cols; j++) {
			if(i<10 or i>deleted.rows-10){
				deleted.at<uchar>(i, j) = 0;
			}
			else {
				deleted.at<uchar>(i, j) = img.at<uchar>(i, j);
			}
		}
	}
	return deleted;
}

Mat img_processing::LPF(Mat img, vector<vector<int>> filter) {
	//Création d'une image identique avec  bordures répliqués
	int border = 1;
	Mat img_lpf(img.rows+border*2, img.cols+border*2, CV_8UC1, Scalar(70));
	copyMakeBorder(img, img_lpf, border, border, border, border, BORDER_REPLICATE);

	//Application du filtre pour chaques pixels de l'image avec les bordures répliquées
	for (int i = 0+ trunc(filter.size() / 2); i < img_lpf.rows-trunc(filter.size()/2); i++) {
		for (int j = 0 + trunc(filter.size() / 2); j < img_lpf.cols - trunc(filter.size() / 2); j++) {
			//Application du filtre : 
			img_lpf.at<uchar>(i, j) =
			    (  (int)img_lpf.at<uchar>(i - 1, j - 1) * filter[0][0] + (int)img_lpf.at<uchar>(i - 1, j) * filter[0][1] + (int)img_lpf.at<uchar>(i - 1, j + 1) * filter[0][2]
				+ (int)img_lpf.at<uchar>(i    , j - 1) * filter[1][0] + (int)img_lpf.at<uchar>(i    , j) * filter[1][1] + (int)img_lpf.at<uchar>(i    , j + 1) * filter[1][2]
				+ (int)img_lpf.at<uchar>(i + 1, j - 1) * filter[2][0] + (int)img_lpf.at<uchar>(i + 1, j) * filter[2][1] + (int)img_lpf.at<uchar>(i + 1, j + 1) * filter[2][2]
				)/9;
		}
	}

	//Renvoie l'image sur laquelle le filtre a été appliqué 
	return img_lpf;
}

/*Mat img_processing::DC_DFT(Mat img) {
	//On prend la DFT de l'image en paramètre
	Mat originalFloat;
	img.convertTo(originalFloat, CV_32FC1, 1.0 / 255.0);
	Mat originalComplex[2] = { originalFloat,Mat::zeros(originalFloat.size(),CV_32F) };
	Mat dftReady;
	merge(originalComplex, 2, dftReady);
	Mat dftOfOriginal;
	dft(dftReady, dftOfOriginal, DFT_COMPLEX_OUTPUT);


	//Convertissage de la DFT pour pouvoir l'afficher
	Mat splitArray[2] = { Mat::zeros(dftOfOriginal.size(),CV_32F),Mat::zeros(dftOfOriginal.size(),CV_32F) };
	split(dftOfOriginal, splitArray);
	Mat dftMagnitude;
	magnitude(splitArray[0], splitArray[1], dftMagnitude);
	log(dftMagnitude, dftMagnitude);
	normalize(dftMagnitude, dftMagnitude, 0, 1, NORM_MINMAX);

	//Recentrage de la DFT convertit
	int centerX = dftMagnitude.cols / 2;
	int centerY = dftMagnitude.rows / 2;

	Mat q1(dftMagnitude, Rect(0, 0, centerX, centerY));
	Mat q2(dftMagnitude, Rect(centerX, 0, centerX, centerY));
	Mat q3(dftMagnitude, Rect(0, centerY, centerX, centerY));
	Mat q4(dftMagnitude, Rect(centerX, centerY, centerX, centerY));

	Mat swapMap;
	q1.copyTo(swapMap);
	q4.copyTo(q1);
	swapMap.copyTo(q4);
	q2.copyTo(swapMap);
	q3.copyTo(q2);
	swapMap.copyTo(q3);

	//Affichage de la DFT convertit et recentrée
	imshow("DFT", dftMagnitude);
	//waitKey(0);

	//Supression de lignes verticales de la DFT
	for (int i = 0; i < dftMagnitude.rows; i++) {
		for (int j = 0; j < dftMagnitude.cols; j++) {
			if (i > round(dftMagnitude.rows / 2)-5 and i < round(dftOfOriginal.rows / 2) +5) {
				dftMagnitude.at<int>(i, j) = 0;
			}
		}
	}
	//Affichage de la DFT avec la zone en moins
	imshow("new_dft", dftMagnitude);
	//TODO Reconvert dftManitude to dftOfOriginal ????
	//Problème au niveau de la reconversion de la dft modifiée pour pouvoir l'inverser par la suite

	//Inversion de la dft
	Mat invertedDFT;
	dft(dftOfOriginal, invertedDFT, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	//Affichage et renvoie de la DFT inversée
	imshow("Inverted DFT", invertedDFT);
	waitKey(0);
	return invertedDFT;
}

*/


int img_processing::count_lines(Mat img) {
	int counter_lines = 0;
	//Comptage des variations pour la première colonne de pixel
	int px_prec = 0;
	for (int j = 0; j < img.rows; j++) {
		int px = (int)img.at<uchar>(j, 0);
		if (px != px_prec && px == 255) {
			counter_lines = counter_lines + 1;
		}
		px_prec = px;
	}
	return counter_lines;
}

int img_processing::count_mean(Mat img) {
	//Calcule des moyennes pour 10 rangées
	vector<int> means;
	for (int j = 0; j < img.rows; j++) {
		int sum_row_i = 0;
		//Moyenne de la rangée
		for (int i = 0; i < img.cols; i++) {
			sum_row_i = sum_row_i + (int)img.at<uchar>(j, i);
		}
		int mean_row_i = sum_row_i / img.cols;
		means.push_back(mean_row_i);
	}
	int counter = 0;
	int mean_all = 0;
	vector<int> means_10;
	for (int i = 5; i < means.size() - 5; i++) {
		int means_10_i = 0;
		for (int j = -5; j < 5; j++) {
			means_10_i = means_10_i + means[i + j];
		}
		means_10_i = means_10_i / 10;
		if (means_10_i != 0) {
			counter = counter + 1;
			mean_all = mean_all + means_10_i;
		}
		means_10.push_back(means_10_i);
	}
	mean_all = mean_all / counter;

	//Affichage des moyennes
	for (int i = 0; i < means_10.size(); i++) {
		cout << means_10[i] << endl;
	}

	//Calcule et affichage de la moyenne, du sd et du mode
	float mean = 0;
	int counter_0 = 0;
	pair<int, int> count_mode = make_pair(-1, -1);
	vector<int> grey_values(256, 0);
	for (int j = 0; j < means_10.size(); j++) {
		if (means_10[j] != 0) {
			counter_0 = counter_0 + 1;
			mean = mean + means_10[j];
			grey_values[means_10[j]] = grey_values[means_10[j]] + 1;
			if (grey_values[means_10[j]] > count_mode.first) {
				count_mode.first = grey_values[means_10[j]];
				count_mode.second = means_10[j];
			}
		}
	}
	mean = mean / counter_0;
	float sxm;
	for (int j = 0; j < means_10.size(); j++) {
		if (means_10[j] != 0) {
			sxm = (means_10[j] * mean) * (means_10[j] * mean);
		}
	}
	float sd = sqrt(sxm / (counter_0 - 1));
	cout << "Mean : " << mean << "\nSD : " << sd << "\nMode : " << count_mode.second << endl;

	//Comptage de marche
	int steps = 0;
	if (sd > 10 || count_mode.second > 250) { //*mean *0.5mean
		//Methode 1, on utilie la moyenne et 0.5*moyenne comme seuil
		cout << "FIRST METHODE \n";
		bool passed_by_low = true;
		for (int i = 0; i < means_10.size(); i++) {
			if (means_10[i] > mean_all * 1 && passed_by_low == true) {
				steps = steps + 1;
				passed_by_low = false;
			}
			if (means_10[i] < mean_all * 0.5) {
				passed_by_low = true;
			}
		}
	}
	if (sd < 10 || sd == 10) {
		if (count_mode.second > 9 && count_mode.second<250) { //*mode *?mode
			cout << "Second METHODE \n";
			//Methode 2, on utilise le mode comme seuil
			bool passed_by_low = true;
			for (int i = 0; i < means_10.size(); i++) {
				if (means_10[i] > count_mode.second * 1 && passed_by_low == true) {
					steps = steps + 1;
					passed_by_low = false;
				}
				if (means_10[i] < count_mode.second * 1 || means_10[i] == count_mode.second * 1) {
					passed_by_low = true;
				}
			}
		}
		if (count_mode.second < 9 || count_mode.second ==9) {
			cout << "Third METHODE \n";
			//Methode 3 on compte les lignes 
			Mat img_lines;
			dilate(img, img_lines, getStructuringElement(MORPH_RECT, Size((img.cols * 20), 1)));
			imshow("Lines to count", img_lines);
			steps = this->count_lines(img_lines);
		}
	}
	cout << "\nSTEPS: " << steps;
	return 0;
}

