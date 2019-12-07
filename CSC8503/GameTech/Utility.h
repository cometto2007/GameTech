#pragma once
#include <stdlib.h>

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

static void DrawDebugLines(vector<Vector3> nodes) {
	for (int i = 1; i < nodes.size(); ++i) {
		Vector3 a = nodes[i - 1];
		Vector3 b = nodes[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}

static void DrawDebugLinesFromPath(NavigationPath path) {
	vector<Vector3> nodes;
	Vector3 pos;
	while (path.PopWaypoint(pos)) {
		nodes.push_back(pos);
	}
	DrawDebugLines(nodes);
}
