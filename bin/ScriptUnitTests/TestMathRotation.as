/*
 Tests and example configuration.
*/

namespace RotationMathTests {
	float test_x =  float(0x219AEF) / (1 << 24), test_y =  float(0x735659) / (1 << 24), test_z = float(-0x2B8D51) / (1 << 26), test_s =  float(0xE1CCB1) / (1 << 24); // A unit quaternion.
	float  alt_x =  float(0xC01609) / (1 << 25),  alt_y =  float(0xFC566B) / (1 << 25),  alt_z = float( 0x409287) / (1 << 24),  alt_s =  float(0x5F2997) / (1 << 23); // Another similar.
	// These were pulled from a arbirarily rotated cube in Second Life using Float2Hex to convert to C99 hexfloats.
	// However, AS 2.23.x misinterprets C99 hexfloats - it sees them as just plain integer hex numbers.  So these had to be manually converted: for instance 0x219AEFp-24 becomes float(0x219AEF) / (1 << 24)
	
	float ROOT_2_OVER_2 = 0.70710678118654752440084436210485f;
	float PI = 3.1415926535897932384626433832795f;
	float PI_BY_TWO = 1.5707963267948966192313216916398f;
	float TWO_PI = 6.283185307179586476925286766559f;
	
	float DEG_TO_RAD = 0.0174532925199433f;
	float RAD_TO_DEG = 57.29577951308233f;
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Calls and runs all the tests in this set.
	*/
	void ExecuteTests() {
		// Verify test values are near where expected:
		UnitTest::EXPECT_NEAR(test_x,  0.131270f);
		UnitTest::EXPECT_NEAR(test_y,  0.450536f);
		UnitTest::EXPECT_NEAR(test_z, -0.042531f);
		UnitTest::EXPECT_NEAR(test_s,  0.882029f);
		UnitTest::EXPECT_NEAR( alt_x,  0.375168f);
		UnitTest::EXPECT_NEAR( alt_y,  0.492846f);
		UnitTest::EXPECT_NEAR( alt_z,  0.252235f);
		UnitTest::EXPECT_NEAR( alt_s,  0.743456f);
		
		TestCreationDestruction();
		TestBasicCtorAndPropertyReads();
		TestPropertyWriteRead();
		TestLiteralCtorAndPropertyReads();
		TestLiteralYPRCtorAndPropertyReads();
		TestEulerVectorCtorAndPropertyReads();
		TestAxisAngleCtorAndPropertyReads();
		TestCopyCtor();
		
		TestImplictConversion();
		TestExplictConversion();
		
		//TestMethodToEuler(); // Only stubbed in C++ (ScriptMath.cpp) - if you really need it, perhaps implement it?
		TestMethodToAxis();
		TestMethodToAngle();
		TestMethodSlerp();
		TestMethodSlerpCopy();
		TestMethodAngleTo();
		
		TestOpMultiply();
		TestOpDivision();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Verify that default ctors actually can be called.  Generally a compilation test, but also does an implicit test of the actual functionality of the registered interface.
	*/
	void TestCreationDestruction() {
		{Engine::Rotation test_var;}
		{Engine::Rotation test_var();}
		{Engine::Quaternion test_var;}
		{Engine::Quaternion test_var();}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test default constructors, but this time read out and verify that the properties have the expected values.
	*
	* By itself this test can't tell if there's an issue with the properties, but combined with TestPropertyWriteRead() it should be pretty good.
	*/
	void TestBasicCtorAndPropertyReads() {
		// Test without parens
		{
			Engine::Rotation test_var;
			UnitTest::EXPECT_EQ(test_var.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var.y, 0.0f);
			UnitTest::EXPECT_EQ(test_var.z, 0.0f);
			UnitTest::EXPECT_EQ(test_var.s, 1.0f);
		}
		{
			Engine::Quaternion test_var;
			UnitTest::EXPECT_EQ(test_var.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var.y, 0.0f);
			UnitTest::EXPECT_EQ(test_var.z, 0.0f);
			UnitTest::EXPECT_EQ(test_var.s, 1.0f);
		}
		// Test with parens
		{
			Engine::Rotation test_var();
			UnitTest::EXPECT_EQ(test_var.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var.y, 0.0f);
			UnitTest::EXPECT_EQ(test_var.z, 0.0f);
			UnitTest::EXPECT_EQ(test_var.s, 1.0f);
		}
		{
			Engine::Quaternion test_var();
			UnitTest::EXPECT_EQ(test_var.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var.y, 0.0f);
			UnitTest::EXPECT_EQ(test_var.z, 0.0f);
			UnitTest::EXPECT_EQ(test_var.s, 1.0f);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Verify that properties can be written to and retain what they were assigned.
	*/
	void TestPropertyWriteRead() {
		{
			Engine::Rotation test_var();
			test_var.x = 1.125f; UnitTest::EXPECT_EQ(test_var.x, 1.125f);
			test_var.y = 2.125f; UnitTest::EXPECT_EQ(test_var.y, 2.125f);
			test_var.z = 3.125f; UnitTest::EXPECT_EQ(test_var.z, 3.125f);
			test_var.s = 4.125f; UnitTest::EXPECT_EQ(test_var.s, 4.125f);
		}
		{
			Engine::Quaternion test_var();
			test_var.x = 1.125f; UnitTest::EXPECT_EQ(test_var.x, 1.125f);
			test_var.y = 2.125f; UnitTest::EXPECT_EQ(test_var.y, 2.125f);
			test_var.z = 3.125f; UnitTest::EXPECT_EQ(test_var.z, 3.125f);
			test_var.s = 4.125f; UnitTest::EXPECT_EQ(test_var.s, 4.125f);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the literal direct-property-write constructor, verifying that the properties have the expected values.
	*/
	void TestLiteralCtorAndPropertyReads() {
		// Explicit rotation using unit values.
		{
			Engine::Rotation test_var(test_x, test_y, test_z, test_s);
			UnitTest::EXPECT_NEAR(test_var.x, test_x);
			UnitTest::EXPECT_NEAR(test_var.y, test_y);
			UnitTest::EXPECT_NEAR(test_var.z, test_z);
			UnitTest::EXPECT_NEAR(test_var.s, test_s);
		}
		{
			Engine::Quaternion test_var(test_x, test_y, test_z, test_s);
			UnitTest::EXPECT_NEAR(test_var.x, test_x);
			UnitTest::EXPECT_NEAR(test_var.y, test_y);
			UnitTest::EXPECT_NEAR(test_var.z, test_z);
			UnitTest::EXPECT_NEAR(test_var.s, test_s);
		}
		// Explicit rotation using non unit values - these should NOT be automatically normalized.  In fact, they shouldn't even be touched - hence the use of equals.
		{
			Engine::Rotation test_var(1.0f, 1.0f, 1.0f, 1.0f);
			UnitTest::EXPECT_EQ(test_var.x, 1.0f);
			UnitTest::EXPECT_EQ(test_var.y, 1.0f);
			UnitTest::EXPECT_EQ(test_var.z, 1.0f);
			UnitTest::EXPECT_EQ(test_var.s, 1.0f);
		}
		{
			Engine::Quaternion test_var(1.0f, 1.0f, 1.0f, 1.0f);
			UnitTest::EXPECT_EQ(test_var.x, 1.0f);
			UnitTest::EXPECT_EQ(test_var.y, 1.0f);
			UnitTest::EXPECT_EQ(test_var.z, 1.0f);
			UnitTest::EXPECT_EQ(test_var.s, 1.0f);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the Yaw-Pitch-Roll constructor, verifying that the properties have the expected values.
	*/
	void TestLiteralYPRCtorAndPropertyReads() {
		// Zero rotation validation
		{
			Engine::Rotation test_var(0.0f, 0.0f, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.x, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.y, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.z, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.s, 1.0f);
		}
		{
			Engine::Quaternion test_var(0.0f, 0.0f, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.x, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.y, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.z, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.s, 1.0f);
		}
		// Test that the rotations are evaluated in the correct order (Z, Y, X)
		{
			Engine::Rotation test_var(PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f);
			UnitTest::EXPECT_NEAR(test_var.x, float( 0xEC835F) / (1 << 25)); // 0.461939
			UnitTest::EXPECT_NEAR(test_var.y, float( 0x61F78B) / (1 << 25)); // 0.191341
			UnitTest::EXPECT_NEAR(test_var.z, float( 0xEC835F) / (1 << 25)); // 0.461939
			UnitTest::EXPECT_NEAR(test_var.s, float( 0xBB8799) / (1 << 24)); // 0.732537
		}
		{
			Engine::Quaternion test_var(PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f);
			UnitTest::EXPECT_NEAR(test_var.x, float( 0xEC835F) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.y, float( 0x61F78B) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.z, float( 0xEC835F) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.s, float( 0xBB8799) / (1 << 24));
		}
		
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the Euler vector constructor, verifying that the properties have the expected values.
	*/
	void TestEulerVectorCtorAndPropertyReads() {
		// Zero rotation validation
		{
			Engine::Vector euler(0.0f, 0.0f, 0.0f);
			Engine::Rotation test_var(euler);
			UnitTest::EXPECT_NEAR(test_var.x, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.y, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.z, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.s, 1.0f);
		}
		{
			Engine::Vector euler(0.0f, 0.0f, 0.0f);
			Engine::Quaternion test_var(euler);
			UnitTest::EXPECT_NEAR(test_var.x, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.y, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.z, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.s, 1.0f);
		}
		// Test that the rotations are evaluated in the correct order (Z, Y, X)
		{
			Engine::Vector euler(PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f);
			Engine::Rotation test_var(euler);
			UnitTest::EXPECT_NEAR(test_var.x, float( 0xEC835F) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.y, float( 0x61F78B) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.z, float( 0xEC835F) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.s, float( 0xBB8799) / (1 << 24));
		}
		{
			Engine::Vector euler(PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f);
			Engine::Quaternion test_var(euler);
			UnitTest::EXPECT_NEAR(test_var.x, float( 0xEC835F) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.y, float( 0x61F78B) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.z, float( 0xEC835F) / (1 << 25));
			UnitTest::EXPECT_NEAR(test_var.s, float( 0xBB8799) / (1 << 24));
		}
		
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the axis-angle constructor, verifying that the properties have the expected values.
	*/
	void TestAxisAngleCtorAndPropertyReads() {
		// Zero rotation validation
		{
			Engine::Vector axis(3.1f, 0.0f, 0.0f); // Magnitude of this vector should make no difference in the outcome.
			float angle = 0.0f;
			Engine::Rotation test_var(axis, angle);
			UnitTest::EXPECT_NEAR(test_var.x, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.y, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.z, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.s, 1.0f);
		}
		{
			Engine::Vector axis(3.1f, 0.0f, 0.0f); // Magnitude of this vector should make no difference in the outcome.
			float angle = 0.0f;
			Engine::Quaternion test_var(axis, angle);
			UnitTest::EXPECT_NEAR(test_var.x, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.y, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.z, 0.0f);
			UnitTest::EXPECT_NEAR(test_var.s, 1.0f);
		}
		// Test that the rotations are evaluated in the correct order (Z, Y, X)
		{
			Engine::Vector axis(3.1f, 2.1f, 1.1f); // Magnitude of this vector should make no difference in the outcome.
			float angle = 4.1f;
			Engine::Rotation test_var(axis, angle);
			UnitTest::EXPECT_NEAR(test_var.x, float( 0x5A3961) / (1 << 23)); //  0.704876
			UnitTest::EXPECT_NEAR(test_var.y, float( 0x7A3D39) / (1 << 24)); //  0.477496
			UnitTest::EXPECT_NEAR(test_var.z, float( 0x800F61) / (1 << 25)); //  0.250117
			UnitTest::EXPECT_NEAR(test_var.s, float(-0xEC11B7) / (1 << 25)); // -0.461072
		}
		{
			Engine::Vector axis(3.1f, 2.1f, 1.1f); // Magnitude of this vector should make no difference in the outcome.
			float angle = 4.1f;
			Engine::Quaternion test_var(axis, angle);
			UnitTest::EXPECT_NEAR(test_var.x, float( 0x5A3961) / (1 << 23)); //  0.704876
			UnitTest::EXPECT_NEAR(test_var.y, float( 0x7A3D39) / (1 << 24)); //  0.477496
			UnitTest::EXPECT_NEAR(test_var.z, float( 0x800F61) / (1 << 25)); //  0.250117
			UnitTest::EXPECT_NEAR(test_var.s, float(-0xEC11B7) / (1 << 25)); // -0.461072
		}
		
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the copy constructors.
	* 
	* Makes sure that the copy is not just a pointer to the original, but is indeed a full independent copy - at least as far as the properties go.
	*/
	void TestCopyCtor() {
		{
			Engine::Rotation test_var_1(test_x, test_y, test_z, test_s);
			Engine::Rotation test_var_2(test_var_1);
			UnitTest::EXPECT_EQ(test_var_2.x, test_x);
			UnitTest::EXPECT_EQ(test_var_2.y, test_y);
			UnitTest::EXPECT_EQ(test_var_2.z, test_z);
			UnitTest::EXPECT_EQ(test_var_2.s, test_s);
			
			test_var_1.x = alt_x;
			test_var_1.y = alt_y;
			test_var_1.z = alt_z;
			test_var_1.s = alt_s;
			UnitTest::EXPECT_EQ(test_var_1.x, alt_x);
			UnitTest::EXPECT_EQ(test_var_2.x, test_x);
		}
		{
			Engine::Quaternion test_var_1(test_x, test_y, test_z, test_s);
			Engine::Quaternion test_var_2(test_var_1);
			UnitTest::EXPECT_EQ(test_var_2.x, test_x);
			UnitTest::EXPECT_EQ(test_var_2.y, test_y);
			UnitTest::EXPECT_EQ(test_var_2.z, test_z);
			UnitTest::EXPECT_EQ(test_var_2.s, test_s);
			
			test_var_1.x = alt_x;
			test_var_1.y = alt_y;
			test_var_1.z = alt_z;
			test_var_1.s = alt_s;
			UnitTest::EXPECT_EQ(test_var_1.x, alt_x);
			UnitTest::EXPECT_EQ(test_var_2.x, test_x);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the implict conversions, as currently it is documented that each of the 3
	*/
	void TestImplictConversion() {
		// Using the copy ctor
		{
			Engine::Rotation test_var_1(test_x, test_y, test_z, test_s);
			Engine::Quaternion test_var_2(test_var_1);
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
			UnitTest::EXPECT_EQ(test_var_1.s, test_var_2.s);
		}
		{
			Engine::Quaternion test_var_1(test_x, test_y, test_z, test_s);
			Engine::Rotation test_var_2(test_var_1);
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.s, test_var_2.s);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
		// Using assignment
		{
			Engine::Rotation test_var_1(test_x, test_y, test_z, test_s);
			Engine::Quaternion test_var_2 = test_var_1;
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
			UnitTest::EXPECT_EQ(test_var_1.s, test_var_2.s);
		}
		{
			Engine::Quaternion test_var_1(test_x, test_y, test_z, test_s);
			Engine::Rotation test_var_2 = test_var_1;
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
			UnitTest::EXPECT_EQ(test_var_1.s, test_var_2.s);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the explict conversions, as currently it is documented that each of the vector3 float types are to be freely convertable.
	*/
	void TestExplictConversion() {
		// Using the copy ctor
		{
			Engine::Rotation test_var_1(test_x, test_y, test_z, test_s);
			Engine::Quaternion test_var_2(Quaternion(test_var_1));
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
			UnitTest::EXPECT_EQ(test_var_1.s, test_var_2.s);
		}
		{
			Engine::Quaternion test_var_1(test_x, test_y, test_z, test_s);
			Engine::Rotation test_var_2(Rotation(test_var_1));
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.s, test_var_2.s);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
		// Using assignment
		{
			Engine::Rotation test_var_1(test_x, test_y, test_z, test_s);
			Engine::Quaternion test_var_2 = Quaternion(test_var_1);
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
			UnitTest::EXPECT_EQ(test_var_1.s, test_var_2.s);
		}
		{
			Engine::Quaternion test_var_1(test_x, test_y, test_z, test_s);
			Engine::Rotation test_var_2 = Rotation(test_var_1);
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
			UnitTest::EXPECT_EQ(test_var_1.s, test_var_2.s);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the conversion to Euler angles method.
	*/
	void TestMethodToEuler() {
		Engine::Vector expected(PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f);
		
		{
			Engine::Rotation test_var(expected);
			
			Engine::Vector value(test_var.ToEuler());
			
			UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
			UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
			UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
		}
		{
			Engine::Quaternion test_var(expected);
			
			Engine::Vector value(test_var.ToEuler());
			
			UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
			UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
			UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the Axis part of the conversion to Axis-Angle methods.
	*/
	void TestMethodToAxis() {
		{
			Engine::Vector expected(
				float( 0x8EA377) / (1 << 25),
				float( 0x3D31B5) / (1 << 22),
				float(-0x5C6DD7) / (1 << 26)
			);
			
			{
				Engine::Rotation test_var(test_x, test_y, test_z, test_s);
				
				Engine::Vector value(test_var.ToAxis());
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
			{
				Engine::Quaternion test_var(test_x, test_y, test_z, test_s);
				
				Engine::Vector value(test_var.ToAxis());
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the Angle part of the conversion to Axis-Angle methods
	*/
	void TestMethodToAngle() {
		Engine::Vector axis(PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f, PI_BY_TWO / 2.0f);
		float angle = 4.1f;
		
		float expected = float(0x8BB95) / (1 << 18);
		
		{
			Engine::Rotation test_var(axis, angle);
			
			float value = test_var.ToAngle();
			
			UnitTest::EXPECT_NEAR(value, expected, 0.00001f, "Unexpected value!");
		}
		{
			Engine::Quaternion test_var(axis, angle);
			
			float value = test_var.ToAngle();
			
			UnitTest::EXPECT_NEAR(value, expected, 0.00001f, "Unexpected value!");
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the spherical linear interpolation method.
	*/
	void TestMethodSlerp() {
		{ // Rotation-based tests
			Engine::Rotation test_var_1(test_x, test_y, test_z, test_s);
			Engine::Rotation test_var_2(alt_x, alt_y, alt_z, alt_s);
			
			{ float mix = 0.0f;
				Engine::Rotation expected(test_x, test_y, test_z, test_s);
				
				Engine::Rotation result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				{ // Verify that the rotations remain unmodified
					UnitTest::EXPECT_EQ(test_var_1.x, test_x, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_1.y, test_y, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_1.z, test_z, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_1.s, test_s, "Original rotation modified!");
					
					UnitTest::EXPECT_EQ(test_var_2.x, alt_x, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_2.y, alt_y, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_2.z, alt_z, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_2.s, alt_s, "Original rotation modified!");
				}
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 1.0f;
				Engine::Rotation expected(alt_x, alt_y, alt_z, alt_s);
				
				Engine::Rotation result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 0.5f;
				Engine::Rotation expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					0.2586889182286233f,
					0.48188024803896057f,
					0.10711696379066188f,
					0.8302989827912869f
				);
				
				Engine::Rotation result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = -0.5f;
				Engine::Rotation expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					-0.0017002667037581687f,
					0.4001387898478883f,
					-0.19038034718369945f,
					0.896458412773772f
				);
				
				Engine::Rotation result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 1.5f;
				Engine::Rotation expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					0.4757813980300848f,
					0.4829695192302075f,
					0.38668613301267746f,
					0.6251726017906446f
				);
				
				Engine::Rotation result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
		}
		{ // Quaternion-based tests
			Engine::Quaternion test_var_1(test_x, test_y, test_z, test_s);
			Engine::Quaternion test_var_2(alt_x, alt_y, alt_z, alt_s);
			
			{ float mix = 0.0f;
				Engine::Quaternion expected(test_x, test_y, test_z, test_s);
				
				Engine::Quaternion result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				{ // Verify that the rotations remain unmodified
					UnitTest::EXPECT_EQ(test_var_1.x, test_x, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_1.y, test_y, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_1.z, test_z, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_1.s, test_s, "Original quaternion modified!");
					
					UnitTest::EXPECT_EQ(test_var_2.x, alt_x, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_2.y, alt_y, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_2.z, alt_z, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_2.s, alt_s, "Original quaternion modified!");
				}
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 1.0f;
				Engine::Quaternion expected(alt_x, alt_y, alt_z, alt_s);
				
				Engine::Quaternion result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 0.5f;
				Engine::Quaternion expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					0.2586889182286233f,
					0.48188024803896057f,
					0.10711696379066188f,
					0.8302989827912869f
				);
				
				Engine::Quaternion result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = -0.5f;
				Engine::Quaternion expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					-0.0017002667037581687f,
					0.4001387898478883f,
					-0.19038034718369945f,
					0.896458412773772f
				);
				
				Engine::Quaternion result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 1.5f;
				Engine::Quaternion expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					0.4757813980300848f,
					0.4829695192302075f,
					0.38668613301267746f,
					0.6251726017906446f
				);
				
				Engine::Quaternion result(test_var_1);
				result.Slerp(test_var_2, mix);
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the spherical linear interpolation method.  This version actually creates a new rotation object.
	*/
	void TestMethodSlerpCopy() {
		{ // Rotation-based tests
			Engine::Rotation test_var_1(test_x, test_y, test_z, test_s);
			Engine::Rotation test_var_2(alt_x, alt_y, alt_z, alt_s);
			
			{ float mix = 0.0f;
				Engine::Rotation expected(test_x, test_y, test_z, test_s);
				
				Engine::Rotation result(test_var_1.SlerpCopy(test_var_2, mix));
				
				{ // Verify that the rotations remain unmodified
					UnitTest::EXPECT_EQ(test_var_1.x, test_x, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_1.y, test_y, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_1.z, test_z, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_1.s, test_s, "Original rotation modified!");
					
					UnitTest::EXPECT_EQ(test_var_2.x, alt_x, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_2.y, alt_y, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_2.z, alt_z, "Original rotation modified!");
					UnitTest::EXPECT_EQ(test_var_2.s, alt_s, "Original rotation modified!");
				}
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 1.0f;
				Engine::Rotation expected(alt_x, alt_y, alt_z, alt_s);
				
				Engine::Rotation result(test_var_1.SlerpCopy(test_var_2, mix));
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 0.5f;
				Engine::Rotation expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					0.2586889182286233f,
					0.48188024803896057f,
					0.10711696379066188f,
					0.8302989827912869f
				);
				
				Engine::Rotation result(test_var_1.SlerpCopy(test_var_2, mix));
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = -0.5f;
				Engine::Rotation expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					-0.0017002667037581687f,
					0.4001387898478883f,
					-0.19038034718369945f,
					0.896458412773772f
				);
				
				Engine::Rotation result(test_var_1.SlerpCopy(test_var_2, mix));
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 1.5f;
				Engine::Rotation expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					0.4757813980300848f,
					0.4829695192302075f,
					0.38668613301267746f,
					0.6251726017906446f
				);
				
				Engine::Rotation result(test_var_1.SlerpCopy(test_var_2, mix));
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
		}
		{ // Quaternion-based tests
			Engine::Quaternion test_var_1(test_x, test_y, test_z, test_s);
			Engine::Quaternion test_var_2(alt_x, alt_y, alt_z, alt_s);
			
			{ float mix = 0.0f;
				Engine::Quaternion expected(test_x, test_y, test_z, test_s);
				
				Engine::Quaternion result(test_var_1.SlerpCopy(test_var_2, mix));
				
				{ // Verify that the rotations remain unmodified
					UnitTest::EXPECT_EQ(test_var_1.x, test_x, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_1.y, test_y, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_1.z, test_z, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_1.s, test_s, "Original quaternion modified!");
					
					UnitTest::EXPECT_EQ(test_var_2.x, alt_x, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_2.y, alt_y, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_2.z, alt_z, "Original quaternion modified!");
					UnitTest::EXPECT_EQ(test_var_2.s, alt_s, "Original quaternion modified!");
				}
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 1.0f;
				Engine::Quaternion expected(alt_x, alt_y, alt_z, alt_s);
				
				Engine::Quaternion result(test_var_1.SlerpCopy(test_var_2, mix));
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 0.5f;
				Engine::Quaternion expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					0.2586889182286233f,
					0.48188024803896057f,
					0.10711696379066188f,
					0.8302989827912869f
				);
				
				Engine::Quaternion result(test_var_1.SlerpCopy(test_var_2, mix));
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = -0.5f;
				Engine::Quaternion expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					-0.0017002667037581687f,
					0.4001387898478883f,
					-0.19038034718369945f,
					0.896458412773772f
				);
				
				Engine::Quaternion result(test_var_1.SlerpCopy(test_var_2, mix));
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
			{ float mix = 1.5f;
				Engine::Quaternion expected( // Calculated usung the SLERP Calculator here: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
					0.4757813980300848f,
					0.4829695192302075f,
					0.38668613301267746f,
					0.6251726017906446f
				);
				
				Engine::Quaternion result(test_var_1.SlerpCopy(test_var_2, mix));
				
				UnitTest::EXPECT_NEAR(result.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.z, expected.z, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(result.s, expected.s, 0.00001f, "Unexpected value!");
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests the AngleTo method.
	*/
	void TestMethodAngleTo() {
		{
			Engine::Rotation test_var_1(test_x, test_y, test_z, test_s);
			Engine::Rotation test_var_2(alt_x, alt_y, alt_z, alt_s);
			
			float expected = float(0xD2F34F) / (1 << 24);
			
			float value = test_var_1.AngleTo(test_var_2);
			
			UnitTest::EXPECT_NEAR(value, expected, 0.00001f, "Unexpected value!");
		}
		{
			Engine::Quaternion test_var_1(test_x, test_y, test_z, test_s);
			Engine::Quaternion test_var_2(alt_x, alt_y, alt_z, alt_s);
			
			float expected = float(0xD2F34F) / (1 << 24);
			
			float value = test_var_1.AngleTo(test_var_2);
			
			UnitTest::EXPECT_NEAR(value, expected, 0.00001f, "Unexpected value!");
		}
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the multiplication operator - in this case it is used as a "combining" operator.
	*
	* Note that the order of the operands is VERY important: this operator is not commutative: A*B != B*A
	*/
	void TestOpMultiply() {
		{
			Engine::Rotation test_var_1(3.1f, 2.1f, 1.1f, 0.1f);
			Engine::Rotation test_var_2(4.1f, 5.1f, 6.1f, 7.1f);
			
			Engine::Rotation result(test_var_1 * test_var_2);
			
			UnitTest::EXPECT_NEAR(result.x, float( 0xF3851F) / (1 << 20));
			UnitTest::EXPECT_NEAR(result.y, float( 0xEE8F5B) / (1 << 19));
			UnitTest::EXPECT_NEAR(result.z, float( 0x13851F) / (1 << 20));
			UnitTest::EXPECT_NEAR(result.s, float(-0x1D6B85) / (1 << 16));
		}
		{
			Engine::Quaternion test_var_1(3.1f, 2.1f, 1.1f, 0.1f);
			Engine::Quaternion test_var_2(4.1f, 5.1f, 6.1f, 7.1f);
			
			Engine::Quaternion result(test_var_2 * test_var_1);
			
			UnitTest::EXPECT_NEAR(result.x, float( 0x767AE1) / (1 << 18));
			UnitTest::EXPECT_NEAR(result.y, float( 0x20A3D7) / (1 << 21));
			UnitTest::EXPECT_NEAR(result.z, float( 0xF9EB85) / (1 << 20));
			UnitTest::EXPECT_NEAR(result.s, float(-0x1D6B85) / (1 << 16));
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the divistion operator - in this case it is used as the inverse of the "combining" multiplication operator such that A*B/B == A.
	*
	* Not sure of the validity of this test: IIRC SecondLife's LSL scripting language had an old bug deep in the rotation division operation.
	*/
	void TestOpDivision() {
		{
			Engine::Rotation test_var_1(3.1f, 2.1f, 1.1f, 0.1f);
			Engine::Rotation test_var_2(4.1f, 5.1f, 6.1f, 7.1f);
			
			Engine::Rotation result(test_var_1 / test_var_2);
			
			UnitTest::EXPECT_NEAR(result.x, float( 0xE66665) / (1 << 19));
			UnitTest::EXPECT_NEAR(result.y, float(-0x000001) / (1 << 20));
			UnitTest::EXPECT_NEAR(result.z, float( 0x733333) / (1 << 19));
			UnitTest::EXPECT_NEAR(result.s, float( 0x0F6B85) / (1 << 15));
		}
		{
			Engine::Quaternion test_var_1(3.1f, 2.1f, 1.1f, 0.1f);
			Engine::Quaternion test_var_2(4.1f, 5.1f, 6.1f, 7.1f);
			
			Engine::Quaternion result(test_var_2 / test_var_1);
			
			UnitTest::EXPECT_NEAR(result.x, float(-0x399999) / (1 << 17));
			UnitTest::EXPECT_NEAR(result.y, float( 0x000000) / (1 << 0));
			UnitTest::EXPECT_NEAR(result.z, float(-0xE66665) / (1 << 20));
			UnitTest::EXPECT_NEAR(result.s, float( 0x0F6B85) / (1 << 15));
		}
	}
}
