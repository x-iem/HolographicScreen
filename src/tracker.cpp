#include <tracker.h>
#include <string>

using namespace cv;

FaceTracker::FaceTracker(unsigned int avg_number) :
    averageX(2),
    averageY(2) {

    if (!loadClassifiers()) {
        if (!openCam()) {
        }
    }
}

FaceTracker::~FaceTracker() {
    closeCam();
}

int FaceTracker::openCam() {
    this->cap = VideoCapture(0);

    if (!this->cap.isOpened()) {
        std::cout << "Erreur: Impossible d'ouvrir la caméra" << std::endl;
        return status::CAM_OPEN_FAILED;
    }

    return status::OK;
}

int FaceTracker::loadClassifiers() {
    CascadeClassifier eyeCascade;

    std::string cascadeFaceName = "face_model/haarcascade_frontalface_default.xml"; // Chemin relatif ou absolu
    std::string cascadeEyeName = "face_model/haarcascade_eye.xml"; // Chemin relatif ou absolu

    if (!this->faceCascade.load(cascadeFaceName)) {
        std::cout << "Erreur: Impossible de charger le classifieur Haar" << std::endl;
        return status::CLASSIFIER_LOAD_FAILED;
    }

    if (!eyeCascade.load(cascadeEyeName)) {
        std::cout << "Erreur: Impossible de charger le classifieur Haar" << std::endl;
        return status::CLASSIFIER_LOAD_FAILED;
    }

    return status::OK;
}

void FaceTracker::closeCam() {
    this->cap.release();
}

std::vector<float> FaceTracker::retrieveFacePos() {
    Mat frame;
    Mat gray;
    std::vector<Rect> faces;
    std::vector<float> points;// = {0, 0};

    this->cap >> frame; // Capture une nouvelle frame

    if (frame.empty()) {}

    cvtColor(frame, gray, COLOR_BGR2GRAY); // Conversion en niveaux de gris
    equalizeHist(gray, gray);               // Égalisation de l'historique pour améliorer la détection

    // 4. Détection des visages
    // Parameters: scaleFactor=1.1, minNeighbors=3, flags=0, minSize=30x30
    this->faceCascade.detectMultiScale(gray,faces, 1.1, 4, 0, Size(70, 70));

    if (faces.size() == 0) { return {0, 0, 0}; }

    points = {averageX.addElement(faces[0].x + faces[0].width/2),
              averageY.addElement(faces[0].y + faces[0].height/2),
              (abs(faces[0].height) + abs(faces[0].width))/2.0f};

    //std::cout << cap.get(cv::CAP_PROP_FRAME_WIDTH) << " " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;

    // 5. Dessin des rectangles autour des visages détectés
    for (size_t i = 0; i < faces.size(); i++) {
        Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
        ellipse(frame, center, Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar(255, 0, 255), 4);
    }

    this->frame = frame;
    return points;
}

cv::Mat FaceTracker::getFrame() {
     return this->frame;
}

int FaceTracker::getWidth() { return cap.get(cv::CAP_PROP_FRAME_WIDTH); }
int FaceTracker::getHeight() { return cap.get(cv::CAP_PROP_FRAME_HEIGHT); }
