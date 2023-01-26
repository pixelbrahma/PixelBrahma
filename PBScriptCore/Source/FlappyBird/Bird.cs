using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using PixelBrahma;

namespace Sandbox
{
	// The Bird class
	public class Bird : Entity
	{
		private TransformComponent m_Transform;
		private Rigidbody2DComponent m_Rigidbody;

		float speedY = 50f;
		float speedX = 10f;
		Vector3 m_Velocity = Vector3.Zero;

		// Create player callback
		void OnCreate()
		{
			Console.WriteLine($"Player.OnCreate - {ID}");

			m_Transform = GetComponent<TransformComponent>();
			m_Rigidbody = GetComponent<Rigidbody2DComponent>();
			m_Velocity.X = 1f;
		}

		// Player update function
		void OnUpdate(float timestep)
		{
			// Console.WriteLine($"Player.OnUpdate: {timestep}");

			m_Velocity.Y = 0f;

			if (Input.IsKeyDown(KeyCode.Space))
				m_Velocity.Y = 1f;

			m_Velocity.Y *= speedY * timestep;
			m_Velocity.X *= speedX * timestep;

			m_Rigidbody.ApplyLinearImpulse(m_Velocity.XY, true);

			CheckCollision();

			//Vector3 translation = m_Transform.Translation;
			//translation += velocity * timestep;
			//m_Transform.Translation = translation;
		}

		// Check for obstacle hit
		void CheckCollision()
		{
			
		}

	}
}
