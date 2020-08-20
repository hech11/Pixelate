using System.Runtime.CompilerServices;

namespace Pixelate {


    public abstract class Component {
        public Entity Entity { get; set; }
    }

///////////////////////////////////////////////////// Rendering components /////////////////////////////////////////////////////


    // TODO: Texture bounds
    public class SpriteRendererComponent : Component
    {
        public void SetTint(Vector4 tint)
        {
            SetTint_CPP(Entity.UUID, ref tint);
        }
        public Vector4 GetTint() {
            return GetTint_CPP(Entity.UUID);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTint_CPP(ulong entity, ref Vector4 tint);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector4 GetTint_CPP(ulong entity);
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


    // TODO
    public class RigidBodyComponent : Component {

        public void SetLinearVelocity(Vector2 velocity)
    	{
            SetLinearVelocity_CPP(Entity.UUID, ref velocity);

        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLinearVelocity_CPP(ulong entity, ref Vector2 vel);
    }



}

