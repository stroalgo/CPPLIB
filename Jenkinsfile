pipeline {
  agent { dockerfile true }
  stages {
    stage('Configure') {
      steps {
        sh 'echo "Configuring..."'        
        sh 'mkdir build && cd build && cmake ..'       
      }
    }

    stage('Build') {
      steps {
        sh 'echo "Building..."'        
        sh 'cd build && make'        
      }
    }

    stage('Test') {
      steps {
        sh 'echo "Running Unit Tests..."'        
      }
    }

    stage('SonarQube') {
      agent any
      steps {
        sh 'echo "Scanning Code..."'        
        sh 'sonar-scanner \
            -Dsonar.sources=. \
            -Dsonar.host.url=http://localhost:9000 \
            -Dsonar.token=sqa_c41bc99e331cf80ec434743663944f1be7ec140f'
      }
    }
  }
}