#include "PXpch.h"
#if 0
#include "EditorAnimationPanel.h"

#include <imgui.h>
#include "Pixelate/Imgui/ImSequencer.h"
#include "Pixelate/Imgui/ImCurveEdit.h"
#include <Imgui\imgui_internal.h>


namespace Pixelate {





	static const char* SequencerItemTypeNames[] = { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };


	struct RampEdit : public ImCurveEdit::Delegate
	{
		RampEdit()
		{
			mPts[0][0] = ImVec2(-10.f, 0);
			mPts[0][1] = ImVec2(20.f, 0.6f);
			mPts[0][2] = ImVec2(25.f, 0.2f);
			mPts[0][3] = ImVec2(70.f, 0.4f);
			mPts[0][4] = ImVec2(120.f, 1.f);
			mPointCount[0] = 5;

			mPts[1][0] = ImVec2(-50.f, 0.2f);
			mPts[1][1] = ImVec2(33.f, 0.7f);
			mPts[1][2] = ImVec2(80.f, 0.2f);
			mPts[1][3] = ImVec2(82.f, 0.8f);
			mPointCount[1] = 4;


			mPts[2][0] = ImVec2(40.f, 0);
			mPts[2][1] = ImVec2(60.f, 0.1f);
			mPts[2][2] = ImVec2(90.f, 0.82f);
			mPts[2][3] = ImVec2(150.f, 0.24f);
			mPts[2][4] = ImVec2(200.f, 0.34f);
			mPts[2][5] = ImVec2(250.f, 0.12f);
			mPointCount[2] = 6;
			mbVisible[0] = mbVisible[1] = mbVisible[2] = true;
			mMax = ImVec2(1.f, 1.f);
			mMin = ImVec2(0.f, 0.f);
		}
		size_t GetCurveCount()
		{
			return 3;
		}

		bool IsVisible(size_t curveIndex)
		{
			return mbVisible[curveIndex];
		}
		size_t GetPointCount(size_t curveIndex)
		{
			return mPointCount[curveIndex];
		}

		uint32_t GetCurveColor(size_t curveIndex)
		{
			uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
			return cols[curveIndex];
		}
		ImVec2* GetPoints(size_t curveIndex)
		{
			return mPts[curveIndex];
		}
		virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const { return ImCurveEdit::CurveSmooth; }
		virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
		{
			mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
			SortValues(curveIndex);
			for (size_t i = 0; i < GetPointCount(curveIndex); i++)
			{
				if (mPts[curveIndex][i].x == value.x)
					return (int)i;
			}
			return pointIndex;
		}
		virtual void AddPoint(size_t curveIndex, ImVec2 value)
		{
			if (mPointCount[curveIndex] >= 8)
				return;
			mPts[curveIndex][mPointCount[curveIndex]++] = value;
			SortValues(curveIndex);
		}
		virtual ImVec2& GetMax() { return mMax; }
		virtual ImVec2& GetMin() { return mMin; }
		virtual unsigned int GetBackgroundColor() { return 0; }
		ImVec2 mPts[3][8];
		size_t mPointCount[3];
		bool mbVisible[3];
		ImVec2 mMin;
		ImVec2 mMax;
	private:
		void SortValues(size_t curveIndex)
		{
			auto b = std::begin(mPts[curveIndex]);
			auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
			std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });

		}
	};

	struct MySequence : public ImSequencer::SequenceInterface
	{
		// interface with sequencer

		virtual int GetFrameMin() const {
			return mFrameMin;
		}
		virtual int GetFrameMax() const {
			return mFrameMax;
		}
		virtual int GetItemCount() const { return (int)myItems.size(); }

		virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
		virtual const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
		virtual const char* GetItemLabel(int index) const
		{
			static char tmps[512];
			sprintf_s(tmps, "[%02d] %s", index, SequencerItemTypeNames[myItems[index].mType]);
			return tmps;
		}

		virtual void Get(int index, int** start, int** end, int* type, unsigned int* color)
		{
			MySequenceItem& item = myItems[index];
			if (color)
				*color = 0xFFAA8080; // same color for everyone, return color based on type
			if (start)
				*start = &item.mFrameStart;
			if (end)
				*end = &item.mFrameEnd;
			if (type)
				*type = item.mType;
		}
		virtual void Add(int type) { myItems.push_back(MySequenceItem{ type, 0, 10, false }); };
		virtual void Del(int index) { myItems.erase(myItems.begin() + index); }
		virtual void Duplicate(int index) { myItems.push_back(myItems[index]); }

		virtual size_t GetCustomHeight(int index) { return myItems[index].mExpanded ? 300 : 0; }

		// my datas
		MySequence() : mFrameMin(0), mFrameMax(0) {}
		int mFrameMin, mFrameMax;
		struct MySequenceItem
		{
			int mType;
			int mFrameStart, mFrameEnd;
			bool mExpanded;
		};
		std::vector<MySequenceItem> myItems;
		RampEdit rampEdit;

		virtual void DoubleClick(int index) {
			if (myItems[index].mExpanded)
			{
				myItems[index].mExpanded = false;
				return;
			}
			for (auto& item : myItems)
				item.mExpanded = false;
			myItems[index].mExpanded = !myItems[index].mExpanded;
		}

		virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
		{
			static const char* labels[] = { "Translation", "Rotation" , "Scale" };

			rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
			rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
			draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
			for (int i = 0; i < 3; i++)
			{
				ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
				ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
				draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
				if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
					rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
			}
			draw_list->PopClipRect();

			ImGui::SetCursorScreenPos(rc.Min);
			float x = rc.Max.x - rc.Min.x;
			float y = rc.Max.y - rc.Min.y;
			ImVec2 xy = { x, y };
			ImCurveEdit::Edit(rampEdit, xy, 137 + index, &clippingRect);
		}

		virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
		{
			rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
			rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
			draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < rampEdit.mPointCount[i]; j++)
				{
					float p = rampEdit.mPts[i][j].x;
					if (p < myItems[index].mFrameStart || p > myItems[index].mFrameEnd)
						continue;
					float r = (p - mFrameMin) / float(mFrameMax - mFrameMin);
					float x = ImLerp(rc.Min.x, rc.Max.x, r);
					draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
				}
			}
			draw_list->PopClipRect();
		}
	};

	inline void rotationY(const float angle, float* m16)
	{
		float c = cosf(angle);
		float s = sinf(angle);

		m16[0] = c;
		m16[1] = 0.0f;
		m16[2] = -s;
		m16[3] = 0.0f;
		m16[4] = 0.0f;
		m16[5] = 1.f;
		m16[6] = 0.0f;
		m16[7] = 0.0f;
		m16[8] = s;
		m16[9] = 0.0f;
		m16[10] = c;
		m16[11] = 0.0f;
		m16[12] = 0.f;
		m16[13] = 0.f;
		m16[14] = 0.f;
		m16[15] = 1.0f;
	}




























	static MySequence mySequence;
	static int currentFrame = 0;
	static bool expanded = false;
	static int firstFrame = 0;

	EditorAnimationPanel::EditorAnimationPanel()
	{
		mySequence.mFrameMin = 0;
		mySequence.mFrameMax = 1000;
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 0, 10, 30, false });
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 1, 20, 30, true });
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 3, 12, 60, false });
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 2, 61, 90, false });
		mySequence.myItems.push_back(MySequence::MySequenceItem{ 4, 90, 99, false });
	}

	void EditorAnimationPanel::OnUpdate(float ts)
	{

	}

	void EditorAnimationPanel::OnEvent(Event& e)
	{

	}

	void EditorAnimationPanel::SetEntityContext(const Entity& entity) {
		m_CurrentEntity = entity;
		m_SelectedAnEntity = true;
	}

	void EditorAnimationPanel::SetEntityContext() {
		m_CurrentEntity = {};
		m_SelectedAnEntity = false;
	}

	void EditorAnimationPanel::OnImguiRender()
	{
		ImGui::Begin("Animation");

		
		

// 		if (m_SelectedAnEntity) {
// 			if (m_CurrentEntity.HasComponent<AnimatorComponent>()) {
// 
// 				auto& animatorComp = m_CurrentEntity.GetComponent<AnimatorComponent>();
// 				Animator& animator = animatorComp.Animator;
// 
// 				std::vector<AnimationClip>& allClips = animator.GetAllClips();
// 
// 	
// 				if (m_CurrentEntity.HasComponent<SpriteRendererComponent>()) {
// 					auto& src = m_CurrentEntity.GetComponent<SpriteRendererComponent>();
// 					Ref<Texture>& texture = src.Texture;
// 					ImGui::Image((ImTextureID)texture->GetHandleID(), { (float)texture->GetWidth(), (float)texture->GetHeight() }, { 0, 1 }, {1, 0});
// 				}
// 
// 
// 
// 				if (allClips.size() > 0) {
// 					ImGui::SameLine(0.0f);
// 
// 					for (AnimationClip& clip : allClips) {
// 						ImGui::Button(clip.GetName().c_str(), { 20,20 });
// 					}
// 					std::vector<AnimationFrame>& allFrames = allClips[0].GetAllFrames();
// 					if (allFrames.size() > 0) {
// 						for (auto& frame : allFrames) {
// 							std::stringstream ss;
// 							ss << "FrameRect: Position(" << frame.FrameRect.Position.x << ", " << frame.FrameRect.Position.y << ")";
// 							ss << "Scale(" << frame.FrameRect.Scale.x << ", " << frame.FrameRect.Scale.y << ")";
// 
// 							ss << "\nFrame timing: " << frame.FrameTiming;
// 
// 							ImGui::Text(ss.str().c_str());
// 
// 						}
// 					}
// 				}
// 				if (ImGui::Button("Add clip")) {
// 					AnimationFrame frame;
// 					AnimationClip clip;
// 					clip.AddFrame(frame);
// 					animator.AddClip(clip);
// 				}
// 
// 
// 			}
// 		}
// 		else {
// 			ImGui::Text("Select an entity with an animator component");
// 		}

		Sequencer(&mySequence, &currentFrame, &expanded, (int*)m_CurrentEntity.GetHandle(), &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
		ImGui::End();
	}

}
#endif