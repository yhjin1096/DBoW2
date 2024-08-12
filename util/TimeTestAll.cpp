#include <chrono>
#include <memory>

#include "DBoW2/DBoW2.h"

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

void loadYmlDB()
{
    Timer t;
    OrbDatabase db;
    db.load("ORBdb.yml.gz");
    printf("load db as yml: %.0fms\n", t.getMilliSec());
}

void loadTextDB()
{
    Timer t;
    OrbVocabulary voc;
    voc.load("ORBvoc.yml.gz");

    OrbDatabase db;
    db.loadFromTextFile(voc, "ORBdb.txt");
    printf("load db as text: %.0fms\n", t.getMilliSec());
}

void loadBinDB()
{
    Timer t;
    OrbVocabulary voc;
    voc.load("ORBvoc.yml.gz");

    OrbDatabase db;
    db.loadFromBinaryFile(voc, "ORBdb.bin");
    printf("load db as bin: %.0fms\n", t.getMilliSec());
}

int main(int argc, char** argv)
{
    loadYmlVoc();
    loadTextVoc();
    loadBinVoc();
    loadYmlDB();
    loadTextDB();
    loadBinDB();
    
    return 0;
}