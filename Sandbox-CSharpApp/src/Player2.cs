using System;
using Pixelate;

public class Player2 : Entity
{
    public int TestingVariable123;
    public int TestingVariable456;
    public int TestingVariable789;
    public int TestingVariable10;
    public int TestingVariable11;


    public void OnCreate()
    {
    }
    public void OnUpdate(float ts)
    {
        float MoveSpeed = 5.0f;

        if (Input.IsKeyDown(KeyCode.D))
        {
            Matrix4x4 transform =  GetTransform();
            transform = Matrix4x4.Translate(transform.Translation.x + MoveSpeed * ts, transform.Translation.y);
            SetTransform(transform);
        }
        else if(Input.IsKeyDown(KeyCode.A))
        {
            Matrix4x4 transform = GetTransform();
            transform = Matrix4x4.Translate(transform.Translation.x - MoveSpeed * ts, transform.Translation.y);
            SetTransform(transform);
        }


        if (Input.IsKeyDown(KeyCode.W))
        {
            Matrix4x4 transform = GetTransform();
            transform = Matrix4x4.Translate(transform.Translation.x, transform.Translation.y + MoveSpeed  * ts);
            SetTransform(transform);
        }
        else if (Input.IsKeyDown(KeyCode.S))
        {
            Matrix4x4 transform = GetTransform();
            transform = Matrix4x4.Translate(transform.Translation.x, transform.Translation.y - MoveSpeed  * ts);
            SetTransform(transform);
        }



    }

    public void OnDestroy()
    {
    }
}


