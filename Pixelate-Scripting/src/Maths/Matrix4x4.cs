using System.Runtime.InteropServices;

namespace Pixelate
{

    [StructLayout(LayoutKind.Explicit)]
    public struct Matrix4x4 {

        [FieldOffset(0)]  private float  Data00;
        [FieldOffset(4)]  private float  Data10;
        [FieldOffset(8)]  private float  Data20;
        [FieldOffset(12)] private float  Data30;
        [FieldOffset(16)] private float  Data01;
        [FieldOffset(20)] private float  Data11;
        [FieldOffset(24)] private float  Data21;
        [FieldOffset(28)] private float  Data31;
        [FieldOffset(32)] private float  Data02;
        [FieldOffset(36)] private float  Data12;
        [FieldOffset(40)] private float  Data22;
        [FieldOffset(44)] private float  Data32;
        [FieldOffset(48)] private float  Data03;
        [FieldOffset(52)] private float  Data13;
        [FieldOffset(56)] private float  Data23;
        [FieldOffset(60)] private float  Data33;


        public Matrix4x4(float value) {
            Data00 = value;
            Data01 = 0.0f; 
            Data02 = 0.0f; 
            Data03 = 0.0f;

            Data10 = 0.0f; 
            Data11 = value;
            Data12 = 0.0f; 
            Data13 = 0.0f;

            Data20 = 0.0f; 
            Data21 = 0.0f; 
            Data22 = value;
            Data23 = 0.0f;

            Data30 = 0.0f;
            Data31 = 0.0f;
            Data32 = 0.0f;
            Data33 = value;

        }


        public Vector2 Translation {
            get { return new Vector2(Data03, Data13); }
            set { Data03 = value.x; Data13 = value.y; }
        }


        public static Matrix4x4 Translate(float x, float y) {
            Matrix4x4 result = new Matrix4x4(1.0f);
            result.Data03 = x;
            result.Data13 = y;
            result.Data23 = 0.0f;
            return result;

        }

    }



}


