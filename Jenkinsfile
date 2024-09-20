pipeline {
  agent { dockerfile true }
  stages {
    stage('Configure') {
      steps {
        sh 'echo "Configuring..."'        
        sh 'cmake -B build'       
      }
    }

    stage('Build') {
      steps {
        sh 'echo "Building..."'        
        sh 'make -C build'        
      }
    }

    stage('Test') {
      steps {
        sh 'echo "Running Unit Tests..."'        
      }
    }

     stage('SonarQube') {

      steps {
                sh 'echo "Static C/C++ code analysis ===> CPPCHECK"'
                sh 'cppcheck \
                    --enable=all  \
                    -v \
                    --language=c++ \
                    --suppress=missingIncludeSystem \
                    --inconclusive \
                    --std=c++17 \
                    --platform=win64 \
                    --xml \
                    --xml-version=2  src 2> cppcheck.xml'      

          script {                    
              withSonarQubeEnv('sonarqube_cpplib') {
                sh '/opt/sonar-scanner/bin/sonar-scanner \
                    -Dsonar.sources=src \
                    -Dsonar.projectKey=cpplib \
                    -Dsonar.cfamily.compile-commands=build/compile_commands.json \
                    -Dsonar.cxx.includeDirectories=/usr/include/c++/14,/usr/include,/usr/include/x86_64-linux-gnu/c++/14,/usr/include/x86_64-linux-gnu,/usr/lib/gcc/x86_64-linux-gnu/14/include \
                    -Dsonar.cxx.cppcheck.reportPaths=cppcheck.xml \
                    -Dsonar.verbose=true ' 
              }
          }
      }
        }
  }
}