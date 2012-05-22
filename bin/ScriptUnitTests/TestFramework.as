/*
 Tests and example configuration.
*/

namespace ConfigTestFramework {
	void ExecuteTests() {
		// Run Tests
		VerifyAsserts();
		
		if (gTestStatus) {
			Engine::LOG(Engine::LOG_PRIORITY::INFO, "All tests passed.");
		}
		else {
			Engine::LOG(Engine::LOG_PRIORITY::ERR, "Tests failed!");
		}
	}
	
	void VerifyAsserts() {
		ASSERT_TRUE(true);   EXPECT_TRUE(true);
		ASSERT_FALSE(false); EXPECT_FALSE(false);
		
		int posi = 1, negi = -1, zeri = 0;
		float posf = 1.125f, negf = -1.125f, zerf = 0.0f;
		float posd = 1.125, negd = -1.125, zerd = 0.0;
		
		ASSERT_EQ(posi, posi);       EXPECT_EQ(posi, posi);
		ASSERT_EQ(zeri, zeri);       EXPECT_EQ(zeri, zeri);
		ASSERT_EQ(negi, negi);       EXPECT_EQ(negi, negi);
		ASSERT_EQ(posf, posf);       EXPECT_EQ(posf, posf);
		ASSERT_EQ(zerf, zerf);       EXPECT_EQ(zerf, zerf);
		ASSERT_EQ(negf, negf);       EXPECT_EQ(negf, negf);
		ASSERT_EQ(posd, posd);       EXPECT_EQ(posd, posd);
		ASSERT_EQ(zerd, zerd);       EXPECT_EQ(zerd, zerd);
		ASSERT_EQ(negd, negd);       EXPECT_EQ(negd, negd);
		ASSERT_EQ(true, true);       EXPECT_EQ(true, true);
		ASSERT_EQ(false, false);     EXPECT_EQ(false, false);
		ASSERT_EQ("green", "green"); EXPECT_EQ("green", "green");
		
		ASSERT_NE(negi, posi);       EXPECT_NE(negi, posi);
		ASSERT_NE(zeri, posi);       EXPECT_NE(zeri, posi);
		ASSERT_NE(posi, negi);       EXPECT_NE(posi, negi);
		ASSERT_NE(negf, posf);       EXPECT_NE(negf, posf);
		ASSERT_NE(zerf, posf);       EXPECT_NE(zerf, posf);
		ASSERT_NE(posf, negf);       EXPECT_NE(posf, negf);
		ASSERT_NE(negd, posd);       EXPECT_NE(negd, posd);
		ASSERT_NE(zerd, posd);       EXPECT_NE(zerd, posd);
		ASSERT_NE(posd, negd);       EXPECT_NE(posd, negd);
		ASSERT_NE(false, true);      EXPECT_NE(false, true);
		ASSERT_NE(true, false);      EXPECT_NE(true, false);
		ASSERT_NE("green", "grand"); EXPECT_NE("green", "grand");
		
		// *TODO: Write unit tests for the rest of the framework.  Seeing that they are largely C&P with minor changes, I'm trivailly testing them now, but they need a full suite to check all the details.
		ASSERT_LT(negi, posi); EXPECT_LT(negi, posi);
		
		ASSERT_LE(negi, posi); EXPECT_LE(negi, posi);
		ASSERT_LE(zeri, zeri); EXPECT_LE(zeri, zeri);
		
		ASSERT_GT(posi, negi); EXPECT_GT(posi, negi);
		
		ASSERT_GE(posi, negi); EXPECT_GE(posi, negi);
		ASSERT_GE(zeri, zeri); EXPECT_GE(zeri, zeri);
	}
	
	bool gTestStatus = true;
	
	// Boolean "simple" tests
	void ASSERT_TRUE (const bool&in a, string message = "")  { if (!a)  { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_TRUE failed! a(" + a + "); " + message); Engine::Shutdown(); } }
	void EXPECT_TRUE (const bool&in a, string message = "")  { if (!a)  { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_TRUE failed! a(" + a + "); " + message); gTestStatus = false; } }
	
	void ASSERT_FALSE(const bool&in a, string message = "") { if (a) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_FALSE failed! a(" + a + "); " + message); Engine::Shutdown(); } }
	void EXPECT_FALSE(const bool&in a, string message = "") { if (a) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_FALSE failed! a(" + a + "); " + message); gTestStatus = false; } }
	
	// Equal tests
	void ASSERT_EQ(const uint8  &in a, const uint8  &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const uint16 &in a, const uint16 &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const uint   &in a, const uint   &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const uint64 &in a, const uint64 &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const  int8  &in a, const  int8  &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const  int16 &in a, const  int16 &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const  int   &in a, const  int   &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const  int64 &in a, const  int64 &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const  float &in a, const  float &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const  double&in a, const  double&in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const  bool  &in a, const  bool  &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_EQ(const  string&in a, const  string&in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_EQ failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void EXPECT_EQ(const uint8  &in a, const uint8  &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const uint16 &in a, const uint16 &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const uint   &in a, const uint   &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const uint64 &in a, const uint64 &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const  int8  &in a, const  int8  &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const  int16 &in a, const  int16 &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const  int   &in a, const  int   &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const  int64 &in a, const  int64 &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const  float &in a, const  float &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const  double&in a, const  double&in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const  bool  &in a, const  bool  &in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_EQ(const  string&in a, const  string&in b, string message = "") { if (a != b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_EQ failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	
	// Not equal tests
	void ASSERT_NE(const uint8  &in a, const uint8  &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const uint16 &in a, const uint16 &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const uint   &in a, const uint   &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const uint64 &in a, const uint64 &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const  int8  &in a, const  int8  &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const  int16 &in a, const  int16 &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const  int   &in a, const  int   &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const  int64 &in a, const  int64 &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const  float &in a, const  float &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const  double&in a, const  double&in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const  bool  &in a, const  bool  &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_NE(const  string&in a, const  string&in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_NE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void EXPECT_NE(const uint8  &in a, const uint8  &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const uint16 &in a, const uint16 &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const uint   &in a, const uint   &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const uint64 &in a, const uint64 &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const  int8  &in a, const  int8  &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const  int16 &in a, const  int16 &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const  int   &in a, const  int   &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const  int64 &in a, const  int64 &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const  float &in a, const  float &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const  double&in a, const  double&in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const  bool  &in a, const  bool  &in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_NE(const  string&in a, const  string&in b, string message = "") { if (a == b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_NE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	
	// Less than tests
	void ASSERT_LT(const uint8  &in a, const uint8  &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const uint16 &in a, const uint16 &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const uint   &in a, const uint   &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const uint64 &in a, const uint64 &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const  int8  &in a, const  int8  &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const  int16 &in a, const  int16 &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const  int   &in a, const  int   &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const  int64 &in a, const  int64 &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const  float &in a, const  float &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LT(const  double&in a, const  double&in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void EXPECT_LT(const uint8  &in a, const uint8  &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const uint16 &in a, const uint16 &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const uint   &in a, const uint   &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const uint64 &in a, const uint64 &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const  int8  &in a, const  int8  &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const  int16 &in a, const  int16 &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const  int   &in a, const  int   &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const  int64 &in a, const  int64 &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const  float &in a, const  float &in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LT(const  double&in a, const  double&in b, string message = "") { if (a >= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	
	// Less than or equal tests
	void ASSERT_LE(const uint8  &in a, const uint8  &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const uint16 &in a, const uint16 &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const uint   &in a, const uint   &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const uint64 &in a, const uint64 &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const  int8  &in a, const  int8  &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const  int16 &in a, const  int16 &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const  int   &in a, const  int   &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const  int64 &in a, const  int64 &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const  float &in a, const  float &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_LE(const  double&in a, const  double&in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_LE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void EXPECT_LE(const uint8  &in a, const uint8  &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const uint16 &in a, const uint16 &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const uint   &in a, const uint   &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const uint64 &in a, const uint64 &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const  int8  &in a, const  int8  &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const  int16 &in a, const  int16 &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const  int   &in a, const  int   &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const  int64 &in a, const  int64 &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const  float &in a, const  float &in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_LE(const  double&in a, const  double&in b, string message = "") { if (a > b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_LE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	
	// Greater than tests
	void ASSERT_GT(const uint8  &in a, const uint8  &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const uint16 &in a, const uint16 &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const uint   &in a, const uint   &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const uint64 &in a, const uint64 &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const  int8  &in a, const  int8  &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const  int16 &in a, const  int16 &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const  int   &in a, const  int   &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const  int64 &in a, const  int64 &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const  float &in a, const  float &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GT(const  double&in a, const  double&in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GT failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void EXPECT_GT(const uint8  &in a, const uint8  &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const uint16 &in a, const uint16 &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const uint   &in a, const uint   &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const uint64 &in a, const uint64 &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const  int8  &in a, const  int8  &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const  int16 &in a, const  int16 &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const  int   &in a, const  int   &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const  int64 &in a, const  int64 &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const  float &in a, const  float &in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GT(const  double&in a, const  double&in b, string message = "") { if (a <= b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GT failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	
	// Greater than or equal tests
	void ASSERT_GE(const uint8  &in a, const uint8  &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const uint16 &in a, const uint16 &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const uint   &in a, const uint   &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const uint64 &in a, const uint64 &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const  int8  &in a, const  int8  &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const  int16 &in a, const  int16 &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const  int   &in a, const  int   &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const  int64 &in a, const  int64 &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const  float &in a, const  float &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void ASSERT_GE(const  double&in a, const  double&in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "ASSERT_GE failed! a(" + a + "), b(" + b + "); " + message); Engine::Shutdown(); } }
	void EXPECT_GE(const uint8  &in a, const uint8  &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const uint16 &in a, const uint16 &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const uint   &in a, const uint   &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const uint64 &in a, const uint64 &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const  int8  &in a, const  int8  &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const  int16 &in a, const  int16 &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const  int   &in a, const  int   &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const  int64 &in a, const  int64 &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const  float &in a, const  float &in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
	void EXPECT_GE(const  double&in a, const  double&in b, string message = "") { if (a < b) { Engine::LOG(Engine::LOG_PRIORITY::ERR, "EXPECT_GE failed! a(" + a + "), b(" + b + "); " + message); gTestStatus = false; } }
}
