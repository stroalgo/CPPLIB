pipeline {
  agent { dockerfile true }

  tools {
        maven "Maven"
    }

  parameters{
      choice(name:'BuildType', choices:'Debug\nRelease\nRelWithDebInfo',description:'Which type of build to consider?'
          )
  }

  environment {
              buildTypeLower = "${BuildType.toLowerCase()}"
              NEXUS_VERSION = "nexus3"
              NEXUS_PROTOCOL = "https"
              NEXUS_URL = "172.17.0.1:8081"
              NEXUS_REPOSITORY = "CppLib"
              NEXUS_CREDENTIAL_ID = "nexus_cpplib"
  }
    
  stages {   

    stage('Load Dependencies') {
      steps {
        sh 'echo "create conan profile..."'
        sh 'conan profile detect'
        sh 'echo "Loading..."'   
        script {
                    if (BuildType == 'RelWithDebInfo') {
                      sh 'conan install .  --build=missing  -s "&:build_type=$BuildType" -sbuild_type=Release'
                    }
                    else {
                      sh 'conan install . -sbuild_type=$BuildType --build=missing'
                    }
                }     
       
      }
    }

    stage('Configure') {     
      
      steps {
        sh 'echo "Configuring..."'        
        sh 'cmake --preset conan-$buildTypeLower'       
      }
    }

    stage('Build') {
      steps {
        sh 'echo "Building..."'        
        sh 'cmake --build --preset conan-$buildTypeLower'        
      }
    }

    stage('Test') {
      steps {
        sh 'echo "Running Unit Tests..."'   
        sh 'ctest -V  --test-dir build/$BuildType --output-junit  unitTestReports.xml'     
     
        sh 'echo "Running Coverage Tests..."'
        sh 'ctest -V -T Coverage --test-dir build/$BuildType'
        sh 'gcovr -r build/$BuildType --cobertura-pretty --cobertura --exclude-unreachable-branches --exclude-throw-branches --print-summary --root . --output coverageTestsReports.xml'     
      }
      post {
        success  {
            junit (testResults:'build/$BuildType/unitTestReports.xml', allowEmptyResults : true)
        }
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
                sh 'cd build/$BuildType && scan-build \
                    -plist \
                    -analyze-headers \
                    -o clang_reports make'

                sh 'echo "Static C/C++ code analysis ===> CLANG-TIDY"'  
                catchError(buildResult: 'SUCCESS', stageResult: 'SUCCESS', message: 'Skip in case of cmd failure')
                {
                  sh 'cd build/$BuildType && run-clang-tidy > clang-tidy.txt'
                }

          script {                    
              withSonarQubeEnv('sonarqube_cpplib') {
                sh '/opt/sonar-scanner/bin/sonar-scanner \
                    -Dsonar.sources=src \
                    -Dsonar.projectKey=cpplib \
                    -Dsonar.cfamily.compile-commands=build/$BuildType/compile_commands.json \
                    -Dsonar.cxx.includeDirectories=/usr/include/c++/14,/usr/include,/usr/include/x86_64-linux-gnu/c++/14,/usr/include/x86_64-linux-gnu,/usr/lib/gcc/x86_64-linux-gnu/14/include \
                    -Dsonar.cxx.cppcheck.reportPaths=cppcheck.xml \
                    -Dsonar.cxx.rats.reportPaths=rats.xml \
                    -Dsonar.cxx.clangsa.reportPaths=build/$BuildType/clang_reports/*/*.plist \
                    -Dsonar.cxx.clangtidy.reportPaths=build/$BuildType/clang-tidy.txt \
                    -Dsonar.cxx.xunit.reportPaths=build/$BuildType/unitTestReports.xml \
                    -Dsonar.cxx.cobertura.reportPaths=coverageTestsReports.xml  \
                    -Dsonar.verbose=true ' 
              }
          }
      }
        }


    stage('Package') {
      steps {
        sh 'echo "Packaging..."'        
        sh 'cd  build/$BuildType && make package'        
      }
      post {
        success  {
            archiveArtifacts artifacts: '_PACKAGE_/CppLib-*.sh', onlyIfSuccessful: true
        }
      }
    }

    stage("Publish Package to nexus") {
            steps {
                script {
                    sh 'echo "Publish package to Nexux..."'    
                    // Read POM xml file using 'readMavenPom' step , this step 'readMavenPom' is included in: https://plugins.jenkins.io/pipeline-utility-steps
                    pom = readMavenPom file: "pom.xml";
                    // Find built artifact under target folder
                    filesByGlob = findFiles(glob: "_PACKAGE_/CppLib-*.sh");
                    // Print some info from the artifact found
                    echo "${filesByGlob[0].name} ${filesByGlob[0].path} ${filesByGlob[0].directory} ${filesByGlob[0].length} ${filesByGlob[0].lastModified}"
                    // Extract the path from the File found
                    artifactPath = filesByGlob[0].path;
                    // Assign to a boolean response verifying If the artifact name exists
                    artifactExists = fileExists artifactPath;

                    if(artifactExists) {
                        echo "*** File: ${artifactPath}";

                        nexusArtifactUploader(
                            nexusVersion: NEXUS_VERSION,
                            protocol: NEXUS_PROTOCOL,
                            nexusUrl: NEXUS_URL,
                           // groupId: pom.groupId,
                            //version: pom.version,
                            repository: NEXUS_REPOSITORY,
                            credentialsId: NEXUS_CREDENTIAL_ID,
                            artifacts: [
                                // Artifact generated such as .jar, .ear and .war files.
                                [artifactId: pom.artifactId,
                                classifier: '',
                                file: artifactPath,
                                ],

                                // Lets upload the pom.xml file for additional information for Transitive dependencies
                                // [artifactId: pom.artifactId,
                                // classifier: '',
                                // file: "pom.xml",
                                // type: "pom"]
                            ]
                        );

                    } else {
                        error "*** File: ${artifactPath}, could not be found";
                    }
                }
            }
        }
  } 
  
  
   
  
  
  
  
  
}