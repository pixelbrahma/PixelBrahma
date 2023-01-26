using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using PixelBrahma;

namespace Sandbox
{
	// The Ball class
	public class Ball : Entity
	{
		private TransformComponent m_Transform;
		
		float speed = 15f;
		Vector3 m_Velocity = Vector3.Zero;
		bool goalFLag = false;

		// Create Ball callback
		void OnCreate()
		{
			Console.WriteLine($"Ball Script - {ID}");

			m_Transform = GetComponent<TransformComponent>();
		}

		// Ball update function
		void OnUpdate(float timestep)
		{
			// Console.WriteLine($"Player.OnUpdate: {timestep}");

			if (Input.IsKeyDown(KeyCode.Space))
				Start();

			if (Input.IsKeyDown(KeyCode.G))
				goalFLag = true;

			Vector3 translation = m_Transform.Translation;
			translation += m_Velocity * timestep;
			m_Transform.Translation = translation;

			CheckCollisions();
		}

		// Start the game
		void Start()
		{
			goalFLag = false;

			Random random = new Random();
			int rand = random.Next();

			if (rand % 2 == 0)
				m_Velocity.X = 1;
			else
				m_Velocity.X = -1;

			rand = random.Next();

			if (rand % 2 == 0)
				m_Velocity.Y = 0.5f;
			else
				m_Velocity.Y = -0.5f;

			m_Velocity *= speed;
		}

		// Function to check for interaction
		void CheckCollisions()
		{
			if (m_Transform.Translation.Y > 14 || m_Transform.Translation.Y < -14)
				m_Velocity.Y *= -1;

			if(!goalFLag)
				if(m_Transform.Translation.X > 22 || m_Transform.Translation.X < -22)
					m_Velocity.X *= -1;

			if (m_Transform.Translation.X > 24 || m_Transform.Translation.X < -24)
				m_Velocity = Vector3.Zero;
		}

	}
}
