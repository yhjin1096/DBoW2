#include <chrono>
#include <memory>

#include "DBoW2/DBoW2.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

class Timer {
public:
  Timer() {
    start_ = std::chrono::system_clock::now();
  }
  virtual ~Timer() = default;

  double getMilliSec() {
    const auto dur = std::chrono::system_clock::now() - start_;
    const auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    return msec;
  }

private:
  std::chrono::system_clock::time_point start_;
};

/*-------------------------------------------------------------------------*/
const int NIMAGES = 5;

void changeStructure(const cv::Mat &plain, std::vector<cv::Mat> &out)
{
  out.resize(plain.rows);

  for(int i = 0; i < plain.rows; ++i)
  {
    out[i] = plain.row(i);
  }
}

void loadFeatures(std::vector<std::vector<cv::Mat>> &features)
{
  features.clear();
  features.reserve(NIMAGES);

  cv::Ptr<cv::ORB> orb = cv::ORB::create(1500);

  std::cout << "Extracting ORB features..." << std::endl;
  for(int i = 0; i < NIMAGES; ++i)
  {
    std::stringstream ss;
    ss << "images3/image" << i << ".png";

    cv::Mat image = cv::imread(ss.str(), 0);
    cv::Mat mask;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;

    orb->detectAndCompute(image, mask, keypoints, descriptors);

    features.push_back(std::vector<cv::Mat>());
    changeStructure(descriptors, features.back());
  }
}

void queryImage(OrbDatabase& db)
{
    std::vector<std::vector<cv::Mat>> features;
    loadFeatures(features);

    DBoW2::QueryResults ret;
    for(int i = 0; i < NIMAGES; i++)
    {
        db.query(features[i], ret, 4);

        // ret[0] is always the same image in this case, because we added it to the 
        // database. ret[1] is the second best match.
        std::cout << "Searching for Image " << i << ". " << ret << std::endl;
    }

    std::cout << std::endl;
}

/*-------------------------------------------------------------------------*/

void loadYmlVoc()
{
    Timer t;
    OrbVocabulary voc;
    voc.load("ORBvoc.yml.gz");
    printf("load voc as yml: %.0fms\n", t.getMilliSec());
}

void loadTextVoc()
{
    Timer t;
    OrbVocabulary voc;
    voc.loadFromTextFile("ORBvoc.txt");
    printf("load voc as text: %.0fms\n", t.getMilliSec());
}

void loadBinVoc()
{
    Timer t;
    OrbVocabulary voc;
    voc.loadFromBinaryFile("ORBvoc.bin");
    printf("load voc as bin: %.0fms\n", t.getMilliSec());
}

/*-------------------------------------------------------------------------*/

void loadYmlDB()
{
    Timer t;
    OrbDatabase db;
    db.load("ORBdb.yml.gz");
    printf("load db as yml: %.0fms\n", t.getMilliSec());
    queryImage(db);
}

void loadTextDB()
{
    Timer t;
    OrbVocabulary voc;
    // voc.load("ORBvoc.yml.gz");
    voc.loadFromTextFile("ORBvoc.txt");
    // voc.loadFromBinaryFile("ORBvoc.bin");

    OrbDatabase db;
    db.loadFromTextFile(voc, "ORBdb.txt");
    printf("load db as text: %.0fms\n", t.getMilliSec());
    queryImage(db);
}

void loadBinDB()
{
    Timer t;
    OrbVocabulary voc;
    // voc.load("ORBvoc.yml.gz");
    // voc.loadFromTextFile("ORBvoc.txt");
    voc.loadFromBinaryFile("ORBvoc.bin");

    OrbDatabase db;
    db.loadFromBinaryFile(voc, "ORBdb.bin");
    printf("load db as bin: %.0fms\n", t.getMilliSec());
    queryImage(db);
}

int main(int argc, char** argv)
{
    std::cout << "opencv version: " << cv::getVersionString() << std::endl;
    // loadYmlVoc();
    // loadTextVoc();
    // loadBinVoc();
    // loadYmlDB();
    // loadTextDB();
    loadBinDB();
    
    return 0;
}