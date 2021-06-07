using System.Runtime.CompilerServices;

namespace Pixelate {


    public abstract class Component {
        public Entity Entity { get; set; }
    }

///////////////////////////////////////////////////// Rendering components /////////////////////////////////////////////////////

    public class SpriteRendererComponent : Component
    {
        public void SetTint(Vector4 tint)
        {
            SetTint_CPP(Entity.UUID, ref tint);
        }
        public Vector4 GetTint() {
            return GetTint_CPP(Entity.UUID);
        }

        public void SetTextureRect(Vector4 coords) {
            //SetTextureRect_CPP(Entity.UUID, ref coords);
        }

        public Vector4 GetTextureRect() {
            return new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
            //return GetTextureRect_CPP(Entity.UUID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTint_CPP(ulong entity, ref Vector4 tint);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector4 GetTint_CPP(ulong entity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTextureRect_CPP(ulong entity, ref Vector4 coords);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector4 GetTextureRect_CPP(ulong entity);
    }

    public class CameraComponent : Component
    {
        public void SetClearColor(Vector4 clearColor)
        {
            SetClearColor_CPP(Entity.UUID, ref clearColor);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetClearColor_CPP(ulong entity, ref Vector4 clearColor);

    }

///////////////////////////////////////////////////// Audio components /////////////////////////////////////////////////////
    public class AudioSourceComponent : Component
    {


        public void Play() { Play_CPP(Entity.UUID); }
        public bool IsPlaying() { return IsPlaying_CPP(Entity.UUID); }
        public void Pause() { Pause_CPP(Entity.UUID); }
        public void Stop() { Stop_CPP(Entity.UUID); }

        public void SetLooping(bool loop) { SetLooping_CPP(Entity.UUID, loop); }
        public void SetGain(float gain) { SetGain_CPP(Entity.UUID, gain); }

        public float GetPitch() { return GetPitch_CPP(Entity.UUID); }
        public float GetGain() { return GetGain_CPP(Entity.UUID); }
        public bool IsLooping() { return IsLooping_CPP(Entity.UUID); }



        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Play_CPP(ulong entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsPlaying_CPP(ulong entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Pause_CPP(ulong entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Stop_CPP(ulong entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLooping_CPP(ulong entity, bool loop);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetGain_CPP(ulong entity, float gain);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetPitch_CPP(ulong entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetGain_CPP(ulong entity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsLooping_CPP(ulong entity);

    }



    ///////////////////////////////////////////////////// Physics components /////////////////////////////////////////////////////


    // TODO : Constraints?
    public class RigidBodyComponent : Component {

        public enum  SleepingState
        {
            Sleep,
		    Awake,
		    NeverSleep
        };
        public enum CollisionDetectionMode
        {
            Continuous,
    		Discrete
        };
        public enum BodyType
        {
            Static,
	    	Kinematic,
	    	Dynamic
        };


        public void SetLinearVelocity(Vector2 velocity)
    	{
            SetLinearVelocity_CPP(Entity.UUID, ref velocity);
        }

        public Vector2 GetLinearVelocity()
        {
            Vector2 temp = new Vector2();
            GetLinearVelocity_CPP(Entity.UUID, out temp);
            return temp;
        }

        public void SetCollisionDetection(CollisionDetectionMode mode) {
            SetCollisionDetection_CPP(Entity.UUID, ref mode);
        }
        public CollisionDetectionMode GetCollisionDetection() {
            CollisionDetectionMode temp;
            GetCollisionDetection_CPP(Entity.UUID, out temp);
            return temp;
        }

        public void SetSleepingState(SleepingState state) {
            SetSleepingState_CPP(Entity.UUID, ref state);
        }

        public SleepingState GetSleepingState() {
            SleepingState temp;
            GetSleepingState_CPP(Entity.UUID, out temp);
            return temp;
        }

        public void SetBodyType(BodyType type) {
            SetBodyType_CPP(Entity.UUID, ref type);
        }
        public BodyType GetBodyType() {
            BodyType temp;
            GetBodyType_CPP(Entity.UUID, out temp);
            return temp;
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLinearVelocity_CPP(ulong entity, ref Vector2 vel);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetLinearVelocity_CPP(ulong entity, out Vector2 vel);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCollisionDetection_CPP(ulong entity, ref CollisionDetectionMode mode);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCollisionDetection_CPP(ulong entity, out CollisionDetectionMode mode);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSleepingState_CPP(ulong entity, ref SleepingState vel);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetSleepingState_CPP(ulong entity, out SleepingState vel);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetBodyType_CPP(ulong entity, ref BodyType type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetBodyType_CPP(ulong entity, out BodyType type);
    }



}

