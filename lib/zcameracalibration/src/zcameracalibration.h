#ifndef Z3D_CAMERACALIBRATION___ZCAMERACALIBRATION_H
#define Z3D_CAMERACALIBRATION___ZCAMERACALIBRATION_H

#include "zcameracalibration_global.h"

#include <QPointer>
#include <QSharedPointer>
#include <QString>

#include <vector>

#include "opencv2/core/core.hpp"

namespace Z3D
{

class Z3D_CAMERACALIBRATION_SHARED_EXPORT ZCameraCalibration : public QObject
{
    Q_OBJECT

public:
    enum CameraModelType {
        CentralCameraType = 0,
        NonCentralCameraType,
        ObliqueCameraType,
        AxialCameraType,
        UnknownCameraType
    };

    typedef QSharedPointer<Z3D::ZCameraCalibration> Ptr;
    typedef QPointer<Z3D::ZCameraCalibration> WeakPtr;

    explicit ZCameraCalibration(CameraModelType cameraModelType);

    virtual ~ZCameraCalibration() {}

    inline bool ready() const { return m_ready; }

    ///
    /// \brief clone
    /// Returns a new object, a copy
    /// \return a new object cloned from this
    ///
    virtual ZCameraCalibration::Ptr clone() const = 0;

    ///
    /// \brief getCalibratedPixel
    /// Returns the calibrated position for the pixel at (x,y)
    /// \param x: "x" coordinate of the pixel
    /// \param y: "y" coordinate of the pixel
    /// \param calibratedX: calibrated "x" coordinate for the pixel
    /// \param calibratedY: calibrated "y" coordinate for the pixel
    /// \return true on success
    ///
    virtual bool getCalibratedPixel(int x, int y,
                                    float *calibratedX, float *calibratedY) = 0;

    ///
    /// \brief getCalibratedSubPixel
    /// \param x: subpixel x coordinate
    /// \param y: subpixel y coordinate
    /// \param calibratedX: calibrated subpixel x coordinate
    /// \param calibratedY: calibrated subpixel y coordinate
    /// \return true on success
    ///
    virtual bool getCalibratedSubPixel(float x, float y,
                                       float *calibratedX, float *calibratedY);

    ///
    /// \brief getRayForPixel
    /// \param x
    /// \param y
    /// \param origin
    /// \param direction
    /// \return true on success
    ///
    virtual bool getRayForPixel(int x, int y,               /// in: pixel coordinates
                                cv::Vec3d &origin,          /// out: ray origin
                                cv::Vec3d &direction) = 0;  /// out: ray direction

    ///
    /// \brief getWorldRayForPixel
    /// \param x
    /// \param y
    /// \param origin
    /// \param direction
    /// \return true on success
    ///
    virtual bool getWorldRayForPixel(int x, int y,              /// in: pixel coordinates
                                     cv::Vec3d &origin,         /// out: ray origin
                                     cv::Vec3d &direction) = 0; /// out: ray direction

    ///
    /// \brief getWorldRayForSubPixel
    /// \param x
    /// \param y
    /// \param origin
    /// \param direction
    /// \return true on sucess
    ///
    virtual bool getWorldRayForSubPixel(float x, float y,      /// in: subpixel coordinates
                                        cv::Vec3d &origin,     /// out: ray origin
                                        cv::Vec3d &direction); /// out: ray direction

    ///
    /// \brief getEstimatedTransformation
    /// \param imageCorners
    /// \param worldCorners
    /// \param rotation
    /// \param translation
    /// \return true on success
    ///
    virtual bool getEstimatedTransformation(std::vector<cv::Point2f> &imageCorners, /// in
                                            std::vector<cv::Point3f> &worldCorners, /// in
                                            cv::Matx33d &rotation,                  /// out
                                            cv::Point3d &translation) = 0;          /// out

    ///
    /// \brief loadCalibration load calibration from file
    /// \param fileName
    /// \return true on success
    ///
    virtual bool loadFromFile(const QString &fileName) = 0;

    ///
    /// \brief saveCalibration saves calibration to file
    /// \param fileName
    /// \return true on success
    ///
    virtual bool saveToFile(const QString &fileName) = 0;

    inline CameraModelType cameraType() const { return m_cameraModelType; }

    inline int sensorWidth() const { return m_sensorWidth; }
    inline int sensorHeight() const { return m_sensorHeight; }

    cv::Matx33d rotation() { return m_rotation; }
    cv::Vec3d translation() { return m_translation; }

    bool setRotation(cv::Matx33d rotation);
    bool setTranslation(cv::Vec3d translation);

signals:
    void calibrationChanged();

    void calibrationReadyChanged(bool ready);
    void calibrationReady();
    void calibrationNotReady();

protected:
    void setReady(bool ready);

    int m_sensorWidth;
    int m_sensorHeight;

    cv::Matx33d m_rotation;
    cv::Vec3d m_translation;

private:
    CameraModelType m_cameraModelType;

    bool m_ready;
};

} // namespace Z3D

#endif // Z3D_CAMERACALIBRATION___ZCAMERACALIBRATION_H