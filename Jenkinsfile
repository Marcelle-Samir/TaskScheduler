pipeline {
    agent any
    stages {
        stage('Checkout') {
            steps {
                // Checkout source code from GitHub
                checkout scm
            }
        }
        stage('Build') {
            steps {
                // Use CMake to build your C++ project
                sh 'cmake . && make'
            }
        }
        stage('Test') {
            steps {
                // Run Google Tests and generate results in XML format
                sh 'pwd'
                sh 'find . -name HitachiScheduler_test'
                sh './build/HitachiScheduler_test --gtest_output=xml:test-results.xml'
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
