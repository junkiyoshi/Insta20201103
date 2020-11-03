#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	ofColor color;
	vector<ofColor> color_list;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		color_list.push_back(color);
	}

	for (int k = 0; k < 15; k++) {

		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
		color = color_list[k % color_list.size()];

		ofMesh mesh;
		vector<glm::vec3> right, left;

		glm::vec3 last_location;
		float last_theta;
		int len = 50;
		int head_size = 15;

		for (int i = 0; i < len; i++) {

			auto location = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i) * 0.005), 0, 1, -350, 350), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i) * 0.005), 0, 1, -350, 350), 0);
			auto next = glm::vec3(ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -350, 350), ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -350, 350), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, len, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, len, 0, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);

			mesh.addColor(ofColor(color, ofMap(i, 0, len, 0, 255)));
			mesh.addColor(ofColor(color, ofMap(i, 0, len, 0, 255)));
		}

		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		auto tmp_head_size = ofMap(len - 2, 0, len, 0, head_size);
		auto tmp_alpha = ofMap(len - 2, 0, len , 0, 255);

		mesh.addVertex(last_location);
		mesh.addColor(ofColor(color, tmp_alpha));
		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
			mesh.addColor(ofColor(color, tmp_alpha));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		mesh.draw();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}