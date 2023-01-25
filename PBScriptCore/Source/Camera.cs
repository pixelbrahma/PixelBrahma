using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using PixelBrahma;

namespace Sandbox
{
	// The camera entity class
	public class Camera : Entity
	{
		// Update function
		void OnUpdate(float ts)
		{
			float speed = 1.0f;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.Up))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.Down))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.Left))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.Right))
				velocity.X = 1.0f;

			velocity *= speed;

			Vector3 translation = Translation;
			translation += velocity * ts;
			Translation = translation;
		}

	}
}
