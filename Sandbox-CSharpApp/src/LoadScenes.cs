using System;
using Pixelate;

public class LoadScenes : Entity
{

    public void OnUpdate(float ts)
    {
        if (Input.IsKeyDown(KeyCode.Num0)) {
            SceneManager.LoadScene("assets/scenes/PhysicsTests.PXScene");
        }

    }

}


