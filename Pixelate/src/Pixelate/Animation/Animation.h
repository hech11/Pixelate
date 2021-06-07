#pragma once


#include "Pixelate/Core/Rect.h"
#include "Pixelate/Core/UUID.h"

#include <vector>
#include <unordered_map>
#include <functional>

namespace Pixelate {

	struct AnimationFrame {
		Rect FrameRect;
		float FrameTiming;

		bool operator==(const AnimationFrame& other) {
			return this->FrameRect == other.FrameRect && this->FrameTiming == other.FrameTiming;
		}
	};

	class AnimationClip {
		public:
			void Update(float ts) {

				m_Timer += ts;
				if (m_Timer >= m_Frames[m_CurrentFrame].FrameTiming) {
					m_CurrentFrame++;
				}

				if (m_CurrentFrame >= m_MaxFrames) {
					m_Timer = 0;
					m_CurrentFrame = 0;
				}

			}

			const Rect& GetCurrentFrameRect() const {
				return m_Frames[m_CurrentFrame].FrameRect;
			}

			inline const std::vector<AnimationFrame>& GetAllFrames() const {
				return m_Frames;
			}

			void AddFrame(Rect framePosition, float frameTiming) {
				m_Frames.push_back({ framePosition, frameTiming });
				m_MaxFrames++;
			}


			void AddFrame(const AnimationFrame& frame) {
				m_Frames.push_back(frame);
				m_MaxFrames++;
			}

			void RemoveFrame(const AnimationFrame& frame) {
				for (int i = 0; i < m_Frames.size(); i++) {
					if (m_Frames[i] == frame) {
						m_Frames.erase(m_Frames.begin()+i);
						break;
					}
				}
			}

			void ResetClipPlayback() {
				m_Timer = 0.0f;
				m_CurrentFrame = 0;
			}

			void ClearAll() {
				m_Frames.clear();
			}

			bool operator==(const AnimationClip& other) {
				return this->m_UUID == other.m_UUID;
			}
			bool operator!=(const AnimationClip& other) {
				return this->m_UUID != other.m_UUID;
			}

		private :
			std::vector<AnimationFrame> m_Frames;

			float m_Timer;
			int m_CurrentFrame = 0;
			int m_MaxFrames;

			UUID m_UUID;

	};

	using AnimationCondition = std::function<bool()>;

	class AnimationTransition {
		public:
			AnimationTransition(const AnimationClip& clipName1, const AnimationClip& clipName2, AnimationCondition predicate) : Clip1(clipName1), Clip2(clipName2), Predicate(predicate) {}

			bool IsPredicateMet() {
				return Predicate();
			}

			void ResetClips() {
				Clip1.ResetClipPlayback();
				Clip2.ResetClipPlayback();
			}


			AnimationClip GetBaseClip() const { return Clip1; }
			AnimationClip Transition() {
				return Clip2;
			}

		private :
			AnimationClip Clip1, Clip2;
			AnimationCondition Predicate;

			bool m_ShouldTransition = false;
	};
	class Animator {

		public :
			void SetDefaultClip(const AnimationClip& clip) {
				m_CurrentClip = clip;
			}
			void AddClip(const AnimationClip& clip) {
				if (m_Clips.size() == 0)
					SetDefaultClip(clip);
				m_Clips.push_back(clip);
			}

			void AddTransition(const AnimationTransition& transition) {
				m_Transitions.push_back(transition);

			}

			AnimationClip GetCurrentClip() const { return m_CurrentClip; }
			void Update(float dt) {
				m_CurrentClip.Update(dt);

				for (int i = 0; i < m_Transitions.size(); i++)
				{
					if (m_Transitions[i].GetBaseClip() == m_CurrentClip && m_Transitions[i].IsPredicateMet()) {
						m_ShouldTransition = true;
						m_TransitionIndex = i;
						break;
					}
				}

				if (m_ShouldTransition) {
					m_Transitions[m_TransitionIndex].ResetClips();
					m_CurrentClip = m_Transitions[m_TransitionIndex].Transition();
					m_ShouldTransition = false;
				}
			}


		private :
			std::vector<AnimationClip> m_Clips;
			AnimationClip m_CurrentClip;

			// replace this with UUIDs
			std::vector<AnimationTransition> m_Transitions;
			bool m_ShouldTransition = false;
			int m_TransitionIndex;
	};

}