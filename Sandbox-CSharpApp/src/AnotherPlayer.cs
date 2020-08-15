using System;

public class AnotherPlayer 
{


    public void OnCreate()
    {
        Console.WriteLine("AnotherPlayer::OnCreate!");
    }
    public void OnUpdate(float ts)
    {
        Console.WriteLine("AnotherPlayer::OnUpdate!: " + ts);

    }

    public void OnDestroy()
    {
        Console.WriteLine("AnotherPlayer::OnDestory!");
    }
}


