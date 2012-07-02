/*
 Tests and example configuration.
*/

namespace Vector3MathTests {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Calls and runs all the tests in this set.
	*/
	void ExecuteTests() {
		TestCreationDestruction();
		TestBasicCtorAndPropertyReads();
		TestPropertyWriteRead();
		TestLiteralCtorAndPropertyReads();
		TestCopyCtor();
		
		TestImplictConversion();
		TestExplictConversion();
		
		TestMethodMagnitudeSq();
		TestMethodMagnitude();
		TestMethodDistanceSq();
		TestMethodDistance();
		TestMethodNormalizedCopy();
		TestMethodNormalize();
		TestMethodDot();
		TestMethodCrossCopy();
		TestMethodCross();
		TestMethodApplyRotationCopy();
		TestMethodApplyRotation();
		TestMethodApplyRotationInvCopy();
		TestMethodApplyRotationInv();
		
		TestOpAddition();
		TestOpSubtraction();
		TestOpMultiplyDotProduct();
		TestOpModCrossProduct();
		TestOpMultiplyApplyRotation();
		TestOpDivisionApplyRotationInv();
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Verify that default ctors actually can be called.  Generally a compilation test, but also does an implicit test of the actual functionality of the registered interface.
	*/
	void TestCreationDestruction() {
		{Engine::Vector test_var;}
		{Engine::Vector test_var();}
		{Engine::Vector3 test_var;}
		{Engine::Vector3 test_var();}
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
			Engine::Vector test_var;
			UnitTest::EXPECT_EQ(test_var.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var.y, 0.0f);
			UnitTest::EXPECT_EQ(test_var.z, 0.0f);
		}
		{
			Engine::Vector3 test_var;
			UnitTest::EXPECT_EQ(test_var.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var.y, 0.0f);
			UnitTest::EXPECT_EQ(test_var.z, 0.0f);
		}
		// Test with parens
		{
			Engine::Vector test_var();
			UnitTest::EXPECT_EQ(test_var.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var.y, 0.0f);
			UnitTest::EXPECT_EQ(test_var.z, 0.0f);
		}
		{
			Engine::Vector3 test_var();
			UnitTest::EXPECT_EQ(test_var.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var.y, 0.0f);
			UnitTest::EXPECT_EQ(test_var.z, 0.0f);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Verify that properties can be written to and retain what they were assigned.
	*/
	void TestPropertyWriteRead() {
		{
			Engine::Vector test_var();
			test_var.x = 1.125f; UnitTest::EXPECT_EQ(test_var.x, 1.125f);
			test_var.y = 2.125f; UnitTest::EXPECT_EQ(test_var.y, 2.125f);
			test_var.z = 3.125f; UnitTest::EXPECT_EQ(test_var.z, 3.125f);
		}
		{
			Engine::Vector3 test_var();
			test_var.x = 1.125f; UnitTest::EXPECT_EQ(test_var.x, 1.125f);
			test_var.y = 2.125f; UnitTest::EXPECT_EQ(test_var.y, 2.125f);
			test_var.z = 3.125f; UnitTest::EXPECT_EQ(test_var.z, 3.125f);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the literal constructors, verifying that the properties have the expected values.
	*/
	void TestLiteralCtorAndPropertyReads() {
		{
			Engine::Vector test_var(1.125f, 2.125f, 3.125f);
			UnitTest::EXPECT_EQ(test_var.x, 1.125f);
			UnitTest::EXPECT_EQ(test_var.y, 2.125f);
			UnitTest::EXPECT_EQ(test_var.z, 3.125f);
		}
		{
			Engine::Vector3 test_var(1.125f, 2.125f, 3.125f);
			UnitTest::EXPECT_EQ(test_var.x, 1.125f);
			UnitTest::EXPECT_EQ(test_var.y, 2.125f);
			UnitTest::EXPECT_EQ(test_var.z, 3.125f);
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
			Engine::Vector test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector test_var_2(test_var_1);
			UnitTest::EXPECT_EQ(test_var_2.x, 1.125f);
			UnitTest::EXPECT_EQ(test_var_2.y, 2.125f);
			UnitTest::EXPECT_EQ(test_var_2.z, 3.125f);
			
			test_var_1.x = 0.0f;
			UnitTest::EXPECT_EQ(test_var_1.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var_2.x, 1.125f);
		}
		{
			Engine::Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector3 test_var_2(test_var_1);
			UnitTest::EXPECT_EQ(test_var_2.x, 1.125f);
			UnitTest::EXPECT_EQ(test_var_2.y, 2.125f);
			UnitTest::EXPECT_EQ(test_var_2.z, 3.125f);
			
			test_var_1.x = 0.0f;
			UnitTest::EXPECT_EQ(test_var_1.x, 0.0f);
			UnitTest::EXPECT_EQ(test_var_2.x, 1.125f);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the implict conversions, as currently it is documented that each of the vector3 float types are to be freely convertable.
	*/
	void TestImplictConversion() {
		// Using the copy ctor
		{
			Vector test_var_1(1.125f, 2.125f, 3.125f);
			Vector3 test_var_2(test_var_1);
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
		{
			Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Vector test_var_2(test_var_1);
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
		// Using assignment
		{
			Vector test_var_1(1.125f, 2.125f, 3.125f);
			Vector3 test_var_2 = test_var_1;
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
		{
			Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Vector test_var_2 = test_var_1;
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test the explict conversions, as currently it is documented that each of the vector3 float types are to be freely convertable.
	*/
	void TestExplictConversion() {
		// Using the copy ctor
		{
			Vector test_var_1(1.125f, 2.125f, 3.125f);
			Vector3 test_var_2(Vector3(test_var_1));
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
		{
			Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Vector test_var_2(Vector(test_var_1));
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
		// Using assignment
		{
			Vector test_var_1(1.125f, 2.125f, 3.125f);
			Vector3 test_var_2 = Vector3(test_var_1);
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
		{
			Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Vector test_var_2 = Vector(test_var_1);
			UnitTest::EXPECT_EQ(test_var_1.x, test_var_2.x);
			UnitTest::EXPECT_EQ(test_var_1.y, test_var_2.y);
			UnitTest::EXPECT_EQ(test_var_1.z, test_var_2.z);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the MagnitudeSq method returns the squared magnitude, aka squared length, of the vector.
	*/
	void TestMethodMagnitudeSq() {
		{
			Engine::Vector test_var(1.125f, 2.125f, 3.125f);
			float value = test_var.MagnitudeSq();
			float expected = 1.125f*1.125f + 2.125f*2.125f + 3.125f*3.125f;
			UnitTest::EXPECT_NEAR(value, expected);
		}
		{
			Engine::Vector3 test_var(1.125f, 2.125f, 3.125f);
			float value = test_var.MagnitudeSq();
			float expected = 1.125f*1.125f + 2.125f*2.125f + 3.125f*3.125f;
			UnitTest::EXPECT_NEAR(value, expected);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the Magnitude method returns the true magnitude, aka length, of the vector.
	*/
	void TestMethodMagnitude() {
		{
			Engine::Vector test_var(1.125f, 2.125f, 3.125f);
			float value = test_var.Magnitude();
			float expected = Math::sqrt(1.125f*1.125f + 2.125f*2.125f + 3.125f*3.125f);
			UnitTest::EXPECT_NEAR(value, expected);
		}
		{
			Engine::Vector3 test_var(1.125f, 2.125f, 3.125f);
			float value = test_var.Magnitude();
			float expected = Math::sqrt(1.125f*1.125f + 2.125f*2.125f + 3.125f*3.125f);
			UnitTest::EXPECT_NEAR(value, expected);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the DistanceSq method returns the squared distance between the vectors.
	*/
	void TestMethodDistanceSq() {
		{
			Engine::Vector test_var_1(6.125f, 2.125f, 4.125f);
			Engine::Vector test_var_2(1.125f, 5.125f, 3.125f);
			
			float delta_x = 6.125f - 1.125f;
			float delta_y = 2.125f - 5.125f;
			float delta_z = 4.125f - 3.125f;
			
			float value = test_var_1.DistanceSq(test_var_2);
			float expected = delta_x*delta_x + delta_y*delta_y + delta_z*delta_z;
			
			UnitTest::EXPECT_NEAR(value, expected);
		}
		{
			Engine::Vector3 test_var_1(6.125f, 2.125f, 4.125f);
			Engine::Vector3 test_var_2(1.125f, 5.125f, 3.125f);
			
			float delta_x = 6.125f - 1.125f;
			float delta_y = 2.125f - 5.125f;
			float delta_z = 4.125f - 3.125f;
			
			float value = test_var_1.DistanceSq(test_var_2);
			float expected = delta_x*delta_x + delta_y*delta_y + delta_z*delta_z;
			
			UnitTest::EXPECT_NEAR(value, expected);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the DistanceSq method returns the true distance between the vectors.
	*/
	void TestMethodDistance() {
		{
			Engine::Vector test_var_1(6.125f, 2.125f, 4.125f);
			Engine::Vector test_var_2(1.125f, 5.125f, 3.125f);
			
			float delta_x = 6.125f - 1.125f;
			float delta_y = 2.125f - 5.125f;
			float delta_z = 4.125f - 3.125f;
			
			float value = test_var_1.Distance(test_var_2);
			float expected = Math::sqrt(delta_x*delta_x + delta_y*delta_y + delta_z*delta_z);
			
			UnitTest::EXPECT_NEAR(value, expected);
		}
		{
			Engine::Vector3 test_var_1(6.125f, 2.125f, 4.125f);
			Engine::Vector3 test_var_2(1.125f, 5.125f, 3.125f);
			
			float delta_x = 6.125f - 1.125f;
			float delta_y = 2.125f - 5.125f;
			float delta_z = 4.125f - 3.125f;
			
			float value = test_var_1.Distance(test_var_2);
			float expected = Math::sqrt(delta_x*delta_x + delta_y*delta_y + delta_z*delta_z);
			
			UnitTest::EXPECT_NEAR(value, expected);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the Normalize method normallizes the vector.
	*/
	void TestMethodNormalize() {
		{
			Engine::Vector test_var(1.125f, 2.125f, 3.125f);
			
			test_var.Normalize();
			
			{ // Verify magnitude as having been normalized.
				UnitTest::EXPECT_NEAR(test_var.Magnitude(), 1.0f);
			}
			
			{ // Deep test the vector.
				float mag = Math::sqrt(1.125f*1.125f + 2.125f*2.125f + 3.125f*3.125f);
				Engine::Vector expected(1.125f / mag, 2.125f / mag, 3.125f / mag);
				
				UnitTest::EXPECT_NEAR(test_var.x, expected.x);
				UnitTest::EXPECT_NEAR(test_var.y, expected.y);
				UnitTest::EXPECT_NEAR(test_var.z, expected.z);
			}
		}
		{
			Engine::Vector3 test_var(1.125f, 2.125f, 3.125f);
			
			test_var.Normalize();
			
			{ // Verify magnitude as having been normalized.
				float mag = test_var.Magnitude();
				UnitTest::EXPECT_NEAR(mag, 1.0f);
			}
			
			{ // Deep test the vector.
				float mag = Math::sqrt(1.125f*1.125f + 2.125f*2.125f + 3.125f*3.125f);
				Engine::Vector expected(1.125f / mag, 2.125f / mag, 3.125f / mag);
				
				UnitTest::EXPECT_NEAR(test_var.x, expected.x);
				UnitTest::EXPECT_NEAR(test_var.y, expected.y);
				UnitTest::EXPECT_NEAR(test_var.z, expected.z);
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the NormalizedCopy method returns a properly normalized copy of the vector.
	*/
	void TestMethodNormalizedCopy() {
		{
			Engine::Vector test_var(1.125f, 2.125f, 3.125f);
			
			Engine::Vector value = test_var.NormalizedCopy();
			
			{ // Deep test the original, verifying that it is NOT changed.
				UnitTest::EXPECT_EQ(test_var.r, 1.125f);
				UnitTest::EXPECT_EQ(test_var.g, 2.125f);
				UnitTest::EXPECT_EQ(test_var.b, 3.125f);
			}
			
			{ // Magnitude on the copy SHOULD be unit.
				UnitTest::EXPECT_NEAR(value.Magnitude(), 1.0f);
			}
			
			{ // Deep test the copy.
				float mag = test_var.Magnitude();
				Engine::Vector expected(
					test_var.x / mag,
					test_var.y / mag,
					test_var.z / mag
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
		{
			Engine::Vector3 test_var(1.125f, 2.125f, 3.125f);
			
			Engine::Vector3 value = test_var.NormalizedCopy();
			
			{ // Deep test the original, verifying that it is NOT changed.
				UnitTest::EXPECT_EQ(test_var.r, 1.125f);
				UnitTest::EXPECT_EQ(test_var.g, 2.125f);
				UnitTest::EXPECT_EQ(test_var.b, 3.125f);
			}
			
			{ // Magnitude on the copy SHOULD be unit.
				UnitTest::EXPECT_NEAR(value.Magnitude(), 1.0f);
			}
			
			{ // Deep test the copy.
				float mag = test_var.Magnitude();
				Engine::Vector3 expected(
					test_var.x / mag,
					test_var.y / mag,
					test_var.z / mag
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the Dot method returns the dot product of the vectors.
	*/
	void TestMethodDot() {
		{
			Engine::Vector test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector test_var_2(6.125f, 5.125f, 4.125f);
			
			float value = test_var_1.Dot(test_var_2);
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			float expected = 1.125f * 6.125f + 2.125f * 5.125f + 3.125f * 4.125f;
			
			UnitTest::EXPECT_NEAR(value, expected);
		}
		{
			Engine::Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector3 test_var_2(6.125f, 5.125f, 4.125f);
			
			float value = test_var_1.Dot(test_var_2);
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			float expected = 1.125f * 6.125f + 2.125f * 5.125f + 3.125f * 4.125f;
			
			UnitTest::EXPECT_NEAR(value, expected);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the Cross method applies the cross product to the left vector.
	*/
	void TestMethodCross() {
		{
			Engine::Vector test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector test_var_2(6.125f, 5.125f, 4.125f);
			
			test_var_1.Cross(test_var_2);
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct cross product in the first vector
				Engine::Vector expected(
					-7.25, // = 2.125f * 4.125f - 3.125f * 5.125f,
					14.50, // = 3.125f * 6.125f - 1.125f * 4.125f,
					-7.25  // = 1.125f * 5.125f - 2.125f * 6.125f
				);
				
				UnitTest::EXPECT_NEAR(test_var_1.x, expected.x);
				UnitTest::EXPECT_NEAR(test_var_1.y, expected.y);
				UnitTest::EXPECT_NEAR(test_var_1.z, expected.z);
			}
		}
		{
			Engine::Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector3 test_var_2(6.125f, 5.125f, 4.125f);
			
			test_var_1.Cross(test_var_2);
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct cross product in the first vector
				Engine::Vector3 expected(
					-7.25, // = 2.125f * 4.125f - 3.125f * 5.125f,
					14.50, // = 3.125f * 6.125f - 1.125f * 4.125f,
					-7.25  // = 1.125f * 5.125f - 2.125f * 6.125f
				);
				
				UnitTest::EXPECT_NEAR(test_var_1.x, expected.x);
				UnitTest::EXPECT_NEAR(test_var_1.y, expected.y);
				UnitTest::EXPECT_NEAR(test_var_1.z, expected.z);
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the CrossCopy method returns the cross product in a new vector.
	*/
	void TestMethodCrossCopy() {
		{
			Engine::Vector test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector test_var_2(6.125f, 5.125f, 4.125f);
			
			Engine::Vector value(test_var_1.CrossCopy(test_var_2));
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct cross product value vector
				Engine::Vector expected(
					-7.25, // = 2.125f * 4.125f - 3.125f * 5.125f,
					14.50, // = 3.125f * 6.125f - 1.125f * 4.125f,
					-7.25  // = 1.125f * 5.125f - 2.125f * 6.125f
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
		{
			Engine::Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector3 test_var_2(6.125f, 5.125f, 4.125f);
			
			Engine::Vector3 value(test_var_1.CrossCopy(test_var_2));
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct cross product value vector
				Engine::Vector3 expected(
					-7.25, // = 2.125f * 4.125f - 3.125f * 5.125f,
					14.50, // = 3.125f * 6.125f - 1.125f * 4.125f,
					-7.25  // = 1.125f * 5.125f - 2.125f * 6.125f
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests to make sure that the rotation is properly applied to the vector.
	*/
	void TestMethodApplyRotation() {
		{
			Engine::Vector value(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			value.ApplyRotation(test_var_2);
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 value(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			value.ApplyRotation(test_var_2);
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector value(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			value.ApplyRotation(test_var_2);
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 value(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			value.ApplyRotation(test_var_2);
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests to make sure that the rotation is properly applied to a copy of the vector.
	*/
	void TestMethodApplyRotationCopy() {
		{
			Engine::Vector test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector value(test_var_1.ApplyRotationCopy(test_var_2));
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector3 value(test_var_1.ApplyRotationCopy(test_var_2));
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector value(test_var_1.ApplyRotationCopy(test_var_2));
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector3 value(test_var_1.ApplyRotationCopy(test_var_2));
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests to make sure that the conjugate of the rotation is properly applied to the vector.
	*/
	void TestMethodApplyRotationInv() {
		{
			Engine::Vector value(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			value.ApplyRotationInv(test_var_2);
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 value(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			value.ApplyRotationInv(test_var_2);
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector value(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			value.ApplyRotationInv(test_var_2);
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 value(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			value.ApplyRotationInv(test_var_2);
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests to make sure that the conjugate of the rotation is properly applied to a copy of the vector.
	*/
	void TestMethodApplyRotationInvCopy() {
		{
			Engine::Vector test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector value(test_var_1.ApplyRotationInvCopy(test_var_2));
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector3 value(test_var_1.ApplyRotationInvCopy(test_var_2));
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector value(test_var_1.ApplyRotationInvCopy(test_var_2));
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector3 value(test_var_1.ApplyRotationInvCopy(test_var_2));
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests to make sure that the addition operator given two vectors returns the sum of the vectors.
	*/
	void TestOpAddition() {
		{
			Engine::Vector test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector test_var_2(6.125f, 5.125f, 4.125f);
			
			Engine::Vector value(test_var_1 + test_var_2);
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct summation vector
				Engine::Vector expected(
					1.125f + 6.125f,
					2.125f + 5.125f,
					3.125f + 4.125f
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
		{
			Engine::Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector3 test_var_2(6.125f, 5.125f, 4.125f);
			
			Engine::Vector3 value(test_var_1 + test_var_2);
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct summation vector
				Engine::Vector3 expected(
					1.125f + 6.125f,
					2.125f + 5.125f,
					3.125f + 4.125f
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests to make sure that the subtraction operator given two vectors returns the difference of the vectors.
	*/
	void TestOpSubtraction() {
		{
			Engine::Vector test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector test_var_2(6.125f, 5.125f, 4.125f);
			
			Engine::Vector value(test_var_1 - test_var_2);
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct summation vector
				Engine::Vector expected(
					1.125f - 6.125f,
					2.125f - 5.125f,
					3.125f - 4.125f
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
		{
			Engine::Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector3 test_var_2(6.125f, 5.125f, 4.125f);
			
			Engine::Vector3 value(test_var_1 - test_var_2);
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct summation vector
				Engine::Vector3 expected(
					1.125f - 6.125f,
					2.125f - 5.125f,
					3.125f - 4.125f
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the multiply operator given two vectors returns the dot product of the vectors.
	*/
	void TestOpMultiplyDotProduct() {
		{
			Engine::Vector test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector test_var_2(6.125f, 5.125f, 4.125f);
			
			float value = test_var_1 * test_var_2;
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			float expected = 1.125f * 6.125f + 2.125f * 5.125f + 3.125f * 4.125f;
			
			UnitTest::EXPECT_NEAR(value, expected);
		}
		{
			Engine::Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector3 test_var_2(6.125f, 5.125f, 4.125f);
			
			float value = test_var_1 * test_var_2;
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			float expected = 1.125f * 6.125f + 2.125f * 5.125f + 3.125f * 4.125f;
			
			UnitTest::EXPECT_NEAR(value, expected);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Test to make sure that the modulo operator given two vectors returns the cross product in a new vector.
	*/
	void TestOpModCrossProduct() {
		{
			Engine::Vector test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector test_var_2(6.125f, 5.125f, 4.125f);
			
			Engine::Vector value(test_var_1 % test_var_2);
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct cross product value vector
				Engine::Vector expected(
					-7.25, // = 2.125f * 4.125f - 3.125f * 5.125f,
					14.50, // = 3.125f * 6.125f - 1.125f * 4.125f,
					-7.25  // = 1.125f * 5.125f - 2.125f * 6.125f
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
		{
			Engine::Vector3 test_var_1(1.125f, 2.125f, 3.125f);
			Engine::Vector3 test_var_2(6.125f, 5.125f, 4.125f);
			
			Engine::Vector3 value(test_var_1 % test_var_2);
			
			{ // Verify that the first vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 1.125f);
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f);
				UnitTest::EXPECT_EQ(test_var_1.z, 3.125f);
			}
			
			{ // Verify that the second vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 6.125f);
				UnitTest::EXPECT_EQ(test_var_2.y, 5.125f);
				UnitTest::EXPECT_EQ(test_var_2.z, 4.125f);
			}
			
			{ // Verify correct cross product value vector
				Engine::Vector3 expected(
					-7.25, // = 2.125f * 4.125f - 3.125f * 5.125f,
					14.50, // = 3.125f * 6.125f - 1.125f * 4.125f,
					-7.25  // = 1.125f * 5.125f - 2.125f * 6.125f
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x);
				UnitTest::EXPECT_NEAR(value.y, expected.y);
				UnitTest::EXPECT_NEAR(value.z, expected.z);
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests to make sure that the multiply operator given a LHS vector and a RHS rotation returns the rotation properly applied to a copy of the vector.
	*/
	void TestOpMultiplyApplyRotation() {
		{
			Engine::Vector test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector value(test_var_1 * test_var_2);
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector3 value(test_var_1 * test_var_2);
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector value(test_var_1 * test_var_2);
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector3 value(test_var_1 * test_var_2);
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0xF722) / (1 << 8),
					float( 0xB4C9) / (1 << 7),
					float( 0x8B01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	* Tests to make sure that the multiply operator given a LHS vector and a RHS rotation returns the conjugate of the rotation properly applied to a copy of the vector.
	*/
	void TestOpDivisionApplyRotationInv() {
		{
			Engine::Vector test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector value(test_var_1 / test_var_2);
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Rotation test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector3 value(test_var_1 / test_var_2);
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector value(test_var_1 / test_var_2);
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
		{
			Engine::Vector3 test_var_1(3.125f, 2.125f, 1.125f);
			Engine::Quaternion test_var_2(7.125f, 6.125f, 5.125f, 4.125f); // Non unit rotation scales as well as rotates - this is expected behavior and proves the correctness of the system better than using a unit quaternion.
			
			Engine::Vector3 value(test_var_1 / test_var_2);
			
			{ // Verify that the vector remains unmodified
				UnitTest::EXPECT_EQ(test_var_1.x, 3.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.y, 2.125f, "Original vector modified!");
				UnitTest::EXPECT_EQ(test_var_1.z, 1.125f, "Original vector modified!");
			}
			
			{ // Verify that the rotation remains unmodified
				UnitTest::EXPECT_EQ(test_var_2.x, 7.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.y, 6.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.z, 5.125f, "Original rotation modified!");
				UnitTest::EXPECT_EQ(test_var_2.s, 4.125f, "Original rotation modified!");
			}
			
			{ // Verify that the resulting vector was rotated correctly
				Engine::Vector3 expected(
					float( 0x9C91) / (1 << 7),
					float( 0xE592) / (1 << 8),
					float( 0xAC01) / (1 << 7) 
				);
				
				UnitTest::EXPECT_NEAR(value.x, expected.x, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.y, expected.y, 0.00001f, "Unexpected value!");
				UnitTest::EXPECT_NEAR(value.z, expected.z, 0.00001f, "Unexpected value!");
			}
		}
	}
	
}
