pipeline {
    agent any
    stages {
        stage('Checkout') {
            steps {
                // Clone your GitHub repository
                checkout scm
            }
        }
        stage('Build') {
            steps {
                // Use a script to build your C++ code
                sh 'g++ -o outputfile src/*.cpp'
            }
        }
        stage('Test') {
            steps {
                // Run Google Tests
                sh './outputfile --gtest_output=xml:test-results.xml'
            }
        }
    }
    post {
        always {
            // Archive the test results
            junit 'test-results.xml'
        }
    }
}
