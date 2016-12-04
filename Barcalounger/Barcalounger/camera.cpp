#include "camera.h"
#include "renderingEngine.h"
#include "Engine.h"

Camera::Camera(const Matrix4f& _projection) :
	projection(_projection) {}

Matrix4f Camera::GetViewProjection() const
{
	Matrix4f cameraRotation = GetTransform().GetTransformedRot().Conjugate().ToRotationMatrix();
	Matrix4f cameraTranslation;

	cameraTranslation.InitTranslation(GetTransform().GetTransformedPos() * -1);

	return projection * cameraRotation * cameraTranslation;
}

void Camera::AddToEngine(CoreEngine* _engine)
{
	_engine->GetRenderingEngine()->AddCamera(this);
}