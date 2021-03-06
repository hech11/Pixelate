using System;
using Pixelate;

public class Player : Entity
{

    public float MoveSpeed = 15.0f;
    public int TempInt = 213;
    public bool TempBool = true;

    public void OnCreate()
    {
        Console.WriteLine(Tag);
    }
    public void OnDestroy()
    {
    }

    public void OnCollisionEnter(Entity collision)
    {
        Console.WriteLine(collision.Tag);
    }

    public void OnCollisionExit(Entity collision)
    {
    }


    public void OnUpdate(float ts)
    {

        
        if (Input.IsMouseButtonDown(MouseButton.Left))
        {
            //GetComponent<RigidBodyComponent>().SetLinearVelocity(new Vector2(0.0f, 5.0f));
        }
        if (Input.IsMouseButtonDown(MouseButton.Right))
        {
            //GetComponent<AudioSourceComponent>().Play();
        }



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

}


