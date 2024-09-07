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
  }
}