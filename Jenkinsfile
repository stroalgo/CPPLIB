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

     stage('SonarQube Analysis') {
            steps {
                script {                    
                    withSonarQubeEnv('sonarqube_cpplib') {
                        sh '/opt/sonar-scanner/bin/sonar-scanner \
                            -Dsonar.sources=. \ 
                            -Dsonar.projectKey=cpplib \
                            -Dsonar.cfamily.compile-commands=build/compile_commands.json'             
                    }
                }
            }
        }
  }
}