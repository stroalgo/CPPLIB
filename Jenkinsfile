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
        sh 'ctest -V --test-dir build --output-junit ../unitTestReports.xml'     
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

                sh 'echo "Static C/C++ code analysis ===> RATS"'  
                sh 'rats \
                    -w 3 \
                    --xml \
                    -l "c" \
                    src > rats.xml'


                sh 'echo "Static C/C++ code analysis ===> CLANG"'  
                sh 'cd build && scan-build \
                    -plist \
                    -analyze-headers \
                    -o ../clang_reports make'

                sh 'echo "Static C/C++ code analysis ===> CLANG-TIDY"'  
                catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS', message: 'Skip in case of cmd failure')
                {
                  sh 'cd build && run-clang-tidy > ../clang-tidy.txt'
                }

          script {                    
              withSonarQubeEnv('sonarqube_cpplib') {
                sh '/opt/sonar-scanner/bin/sonar-scanner \
                    -Dsonar.sources=src \
                    -Dsonar.tests=src \
                    -Dsonar.projectKey=cpplib \
                    -Dsonar.cfamily.compile-commands=build/compile_commands.json \
                    -Dsonar.cxx.includeDirectories=/usr/include/c++/14,/usr/include,/usr/include/x86_64-linux-gnu/c++/14,/usr/include/x86_64-linux-gnu,/usr/lib/gcc/x86_64-linux-gnu/14/include \
                    -Dsonar.cxx.cppcheck.reportPaths=cppcheck.xml \
                    -Dsonar.cxx.rats.reportPaths=rats.xml \
                    -Dsonar.cxx.clangsa.reportPaths=clang_reports/*/*.plist \
                    -Dsonar.cxx.clangtidy.reportPaths=clang-tidy.txt \
                    -Dsonar.cxx.xunit.reportPaths=unitTestReports.xml \
                    -Dsonar.verbose=true ' 
              }
          }
      }
        }
  }
}