#pragma once
#include "../../Common/Vector3.h"
#include "Debug.h"
#include <vector>

namespace NCL {
	using namespace NCL::Maths;
	namespace CSC8503 {
		class NavigationPath		{
		public:
			NavigationPath() {}
			~NavigationPath() {}

			void	Clear() {
				waypoints.clear();
			}
			void	PushWaypoint(const Vector3& wp) {
				waypoints.emplace_back(wp);
			}
			bool	PopWaypoint(Vector3& waypoint) {
				if (waypoints.empty()) {
					return false;
				}
				waypoint = waypoints.back();
				waypoints.pop_back();
				return true;
			}

			void DrawDebugLine() {
				for (int i = 1; i < waypoints.size(); ++i) {
					Vector3 a = waypoints[i - 1];
					Vector3 b = waypoints[i];

					Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
				}
			}

		protected:
			std::vector<Vector3> waypoints;
		};
	}
}
