// Connect your headers so that the code functions
// SDK made by aheartlessman in 2024
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

// Draw Line
void DrawLine(const Vector3& start, const Vector3& end, const ImU32& color) {
	Vector3 startScreen = WorldToScreen(start);
	Vector3 endScreen = WorldToScreen(end);
	if (startScreen.x >= 1.0f && startScreen.y >= 1.0f && startScreen.z >= 1.0f &&
		endScreen.x >= 1.0f && endScreen.y >= 1.0f && endScreen.z >= 1.0f) {
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(startScreen.x, startScreen.y), ImVec2(endScreen.x, endScreen.y), color);
	}
}

// Draw ideal circle
void DrawCircle(const Vector3& center, float radius, ImU32 color) {
	const int segments = 20;
	const float angleIncrement = 2 * 3.14159f / segments;
	Vector3 previousVertex = { center.x + radius, center.y, center.z };
	for (int i = 1; i <= segments; i++) {
		float angle = i * angleIncrement;
		Vector3 currentVertex = {
			center.x + radius * cos(angle),
			center.y + radius * sin(angle),
			center.z
		};
		DrawLine(previousVertex, currentVertex, color);
		previousVertex = currentVertex;
	}
}
// Draw Bounded Box
void DrawBox(const Vector3& min, const Vector3& max, ImU32 color) {
	Vector3 vertices[8] = {
		{min.x, min.y, min.z}, {max.x, min.y, min.z},
		{max.x, max.y, min.z}, {min.x, max.y, min.z},
		{min.x, min.y, max.z}, {max.x, min.y, max.z},
		{max.x, max.y, max.z}, {min.x, max.y, max.z}
	};

	for (int i = 0; i < 4; ++i) {
		DrawLine(vertices[i], vertices[(i + 1) % 4], color);
		DrawLine(vertices[i + 4], vertices[(i + 1) % 4 + 4], color);
		DrawLine(vertices[i], vertices[i + 4], color);
	}
}
// Draw Skeleton with a head
void DrawSkeleton(uint64_t Entity) {
	int boneCount = sizeof(BoneMatrix) / sizeof(BoneMatrix[0]);
	std::vector<Vector3> bonePositions;
	for (int i = 0; i < boneCount; i++) {
		Vector3 bonePosition = GetBone(Entity, BoneMatrix[i][0]);
		bonePositions.push_back(bonePosition);
	}

	for (int i = 0; i < boneCount; i++) {
		Vector3 bonePosition1 = bonePositions[i];

		if (BoneMatrix[i][1] != -1) {
			Vector3 bonePosition2 = GetBone(Entity, BoneMatrix[i][1]);
			Vector3 bonePosition2Screen = WorldToScreen(bonePosition2);
			ImU32 textColor1 = IM_COL32(
				static_cast<unsigned char>(colorSettings.playerSkeletonColor.x * 255),
				static_cast<unsigned char>(colorSettings.playerSkeletonColor.y * 255),
				static_cast<unsigned char>(colorSettings.playerSkeletonColor.z * 255),
				static_cast<unsigned char>(colorSettings.playerSkeletonColor.w * 255)
			); // This is a customizable color, you set it static if you don't have a customizable one.
			DrawLine(bonePosition1, bonePosition2, textColor1);
		}
	}
	int headBoneIndex = 0;
	ImU32 textColor1 = IM_COL32(
		static_cast<unsigned char>(colorSettings.playerSkeletonColor.x * 255),
		static_cast<unsigned char>(colorSettings.playerSkeletonColor.y * 255),
		static_cast<unsigned char>(colorSettings.playerSkeletonColor.z * 255),
		static_cast<unsigned char>(colorSettings.playerSkeletonColor.w * 255)
	); // This is a customizable color, you set it static if you don't have a customizable one.

	Vector3 headPosition = GetBone(Entity, BoneMatrix[headBoneIndex][0]);
	DrawCircle(headPosition, 0.1f, textColor1);
}
