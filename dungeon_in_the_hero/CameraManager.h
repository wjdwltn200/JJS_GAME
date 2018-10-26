#pragma once
class CameraManager : public singletonBase<CameraManager>
{
private:
	SYNTHESIZE(float, m_fCamPosX, CamPosX);
	SYNTHESIZE(float, m_fCamPosY, CamPosY);

public:
	void update(float MinX, float MaxX, float MinY, float MaxY);

	CameraManager();
	~CameraManager();
};

