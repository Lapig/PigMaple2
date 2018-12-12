#pragma once
static struct configInfo {
	bool chestChams = true;
	bool wireFrame = false;
	bool portalChams = true;
	int zoomCap = 1100;
	bool rollCheck = false;
	float viewRoll = 90.f;
	bool yawCheck = false;
	float viewYaw = 45.f;
	bool pitchCheck = false;
	float viewPitch = -40.89387131f;
} hack_config;

int hookCamera(bool state);