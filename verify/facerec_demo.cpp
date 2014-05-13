#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

static Mat toGrayscale(InputArray _src) {
    Mat src = _src.getMat();
    // only allow one channel
    if(src.channels() != 1) {
        CV_Error(CV_StsBadArg, "Only Matrices with one channel are supported");
    }
    // create and return normalized image
    Mat dst;
    cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
    return dst;
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int main(int argc, const char *argv[]) {
    // Check for valid command line arguments, print usage
    // if no arguments were given.
    if (argc != 4) {
      cout << "usage: " << argv[0] << " <face_train.csv> <face_test.csv> <face_result.csv>" << endl;
      exit(1);
    }
    // Get the path to your CSV.
    string tr_fn_csv = string(argv[1]);
    string te_fn_csv = string(argv[2]);
    string res_fn_csv = string(argv[3]);
    // These vectors hold the images and corresponding labels.
    vector<Mat> tr_images;
    vector<int> tr_labels;
    vector<Mat> te_images;
    vector<int> te_labels;
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        read_csv(tr_fn_csv, tr_images, tr_labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << tr_fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    try {
        read_csv(te_fn_csv, te_images, te_labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << te_fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(tr_images.size() <= 1 || te_images.size() < 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size:
    int height = tr_images[0].rows;
    // The following lines simply get the last images from
    // your dataset and remove it from the vector. This is
    // done, so that the training data (which we learn the
    // cv::FaceRecognizer on) and the test data we test
    // the model with, do not overlap.

    //int r=0;
    //float acc;

    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->train(tr_images, tr_labels);
    
    ofstream os;
    os.open(res_fn_csv, std::fstream::out);

    for (size_t i = 0; i < te_images.size(); i++) {
      int predictedLabel = model->predict(te_images[i]);
      
      // print to res file
      os << std::to_string(predictedLabel) << "\n";
    }

    os.close();

    return 0;
}
