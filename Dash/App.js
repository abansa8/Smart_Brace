/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow
 */

import React, { Component, useState, useEffect } from 'react';

import {
  SafeAreaView,
  StyleSheet,
  ScrollView,
  View,
  Text,
  Image,
  StatusBar,
  Dimensions,
  Animated,
  Easing
} from 'react-native';

import {
  Header,
  LearnMoreLinks,
  Colors,
  DebugInstructions,
  ReloadInstructions,
} from 'react-native/Libraries/NewAppScreen';

import {
  LineChart,
  BarChart,
  PieChart,
  ProgressChart,
  ContributionGraph,
  StackedBarChart
} from "react-native-chart-kit";

import {
  ActionButton,
  Avatar,
  ListItem,
  Toolbar,
  BottomNavigation,
  Icon,
  COLOR,
  ThemeContext,
  getTheme
} from 'react-native-material-ui';

import LinearGradient from 'react-native-linear-gradient';
import Spinner from './SpinnerComponent';

import Firebase from 'firebase';

var PushNotification = require("react-native-push-notification");


import styled from 'styled-components'
const keyframes = require('styled-components').keyframes


var firebaseConfig = {
  apiKey: "AIzaSyAzWMeNwyE8OH4L7BLOS6e2DqkraRZMnlo",
  authDomain: "esp32test-acf49.firebaseapp.com",
  databaseURL: "https://esp32test-acf49.firebaseio.com",
  projectId: "esp32test-acf49",
  storageBucket: "esp32test-acf49.appspot.com",
  messagingSenderId: "870795617355",
  appId: "1:870795617355:web:5aa3c606c47f56724799ce",
  measurementId: "G-CZJG22MZQ8"
};
// you can set your style right here, it'll be propagated to application
const uiTheme = {
  palette: {
    primaryColor: COLOR.green500,
  },
  toolbar: {
    container: {
      height: 60,
      backgroundColor: '#002171'
    },
    titleText: {
      fontSize: 30,
      textShadowRadius: 20,
    }
  },
};

const chartConfig = {
  backgroundGradientFrom: "#0d47a1",
  backgroundGradientFromOpacity: 0,
  backgroundGradientTo: "#0d47a1",
  backgroundGradientToOpacity: 0.5,
  color: (opacity = 1) => `rgba(26, 255, 146, ${opacity})`,
  strokeWidth: 2, // optional, default 3
  barPercentage: 0.5
};

const FadeInView = (props) => {
  const [fadeAnim] = useState(new Animated.Value(0))  // Initial value for opacity: 0

  React.useEffect(() => {
    Animated.timing(
      fadeAnim,
      {
        toValue: 1,
        duration: 1000,
      },
    ).start();
  }, [])

  return (
    <Animated.Image                 // Special animatable View
      style={{
        ...props.style,
        opacity: fadeAnim,
        width: 100      // Bind opacity to animated value
      }}
      source={require('./assets/hand.png')}
    >
      {props.children}
    </Animated.Image>
  );
}

export default class HelloWorldApp extends Component {

  state = {
    wristData: { data: [0] },
    bodyData: { data: [0] },
    angleData: { data: [0] },
    labelData: ["1 min"],
    spinAngle: 0,
    lastSpinAngle: 0
  }

  readBodyData() {
    Firebase.database().ref('/').on('value', (snapshot) => {
      this.setState({
        bodyData: { data: [(snapshot.val().body / 10)] }
      })
      console.log(this.state['bodyData']);
    }, function (error) {
      console.log(error);
    });
  }
  readWristData() {
    Firebase.database().ref('/').on('value', (snapshot) => {
      this.setState({
        wristData: { data: [(snapshot.val().wrist / 10)] }
      })
      console.log(this.state['wristData']);
    }, function (error) {
      console.log(error);
    });
  }

  readAngleData() {
    Firebase.database().ref('GyroReadings/').on('value', (snapshot) => {
      console.log(snapshot.val());
      var lengthArr = snapshot.val().length;
      var numEntries = Math.ceil(lengthArr / 60);
      if (numEntries == 1) {
        this.setState({
          labelData: ["0 min", "1 min"]
        })
      }
      else if (numEntries == 2) {
        this.setState({
          labelData: ["0 min", "1 min", "2 min"]
        })
      }
      else if (numEntries == 3) {
        this.setState({
          labelData: ["0 min", "1 min", "2 min", "3 min"]
        })
      }
      else if (numEntries == 4) {
        this.setState({
          labelData: ["0 min", "1 min", "2 min", "3 min", "4 min"]
        })
      }
      else if (numEntries == 5) {
        this.setState({
          labelData: ["0 min", "1 min", "2 min", "3 min", "4 min", "5 min"]
        })
      }
      this.setState({
        angleData: { data: (snapshot.val()) }
      })
      this.setState({
        spinAngle: snapshot.val()[snapshot.val().length - 1]
      })
    }, function (error) {
      console.log(error);
    });
  }

  readVibration() {
    Firebase.database().ref('/').on('value', (snapshot) => {
      if (snapshot.val().vibration == 1) {
        PushNotification.localNotification({
          vibrate: true, // (optional) default: true
          vibration: 300, // vibration length in milliseconds, ignored if vibrate=false, default: 1000


            /* iOS and Android properties */
          title: "Smart Brace: Alert", // (optional)
          message: "Poor wrist angles detected. Please correct your posture", // (required)
          playSound: false, // (optional) default: true
          soundName: 'default', // (optional) Sound to play when the notification is shown. Value of 'default' plays the default sound. It can be set to a custom sound such as 'android.resource://com.xyz/raw/my_sound'. It will look for the 'my_sound' audio file in 'res/raw' directory and play it. default: 'default' (default sound is played)
          actions: '["Yes", "No"]',  // (Android only) See the doc for notification actions to know more
        });
      }
    }, function (error) {
      console.log(error);
    });
  }

  constructor(props) {
    super(props);
    console.log(this.props);
    if (!Firebase.apps.length) {
      Firebase.initializeApp(firebaseConfig);
    }
    this.readBodyData();
    this.readWristData();
    this.readAngleData();
    this.readVibration();
  }

  render() {
    return (
      <>
        {
          <ThemeContext.Provider value={getTheme(uiTheme)}>
            <View>
              <Toolbar
                key="toolbar"
                centerElement="SMART DASH"
              />
              <LinearGradient
                colors={['#0d47a1', '#0d47a1']}
                style={{ padding: 15, alignItems: 'center', height: '100%' }}>
                <View>
                  <Text style={styles.subTitles}>Daily Trend</Text>
                  <LineChart
                    data={{
                      labels: this.state['labelData'],
                      datasets: [
                        this.state['angleData']
                      ]
                    }}
                    width={(Dimensions.get("window").width) * 0.95} // from react-native
                    height={300}
                    chartConfig={{
                      decimalPlaces: 0, // optional, defaults to 2dp
                      backgroundGradientFrom: "#ffffff",
                      backgroundGradientFromOpacity: 1,
                      backgroundGradientTo: "#ffffff",
                      backgroundGradientToOpacity: 1,
                      fillShadowGradient: '#5471d2',
                      fillShadowGradientOpacity: 0.2,
                      color: (opacity = 1) => `rgba(0, 0, 0, ${opacity})`,
                      labelColor: (opacity = 1) => `rgba(0, 0, 0, ${opacity})`,
                      style: {
                        borderRadius: 16
                      },
                      propsForDots: {
                        r: "3",
                        strokeWidth: "1",
                        stroke: "#0d47a1"
                      }
                    }}
                    bezier
                    style={styles.floatView}
                  />
                </View>
                <View style={{ flexDirection: 'row', justifyContent: 'space-between' }}>
                  <View style={{ flexDirection: 'column', width: '100%', justifyContent: 'space-between', paddingTop: 24 }}>
                    <View style={{ textAlign: 'left' }}>
                      <Text style={styles.subTitles2}>Wrist</Text>
                      <ProgressChart
                        data={this.state['wristData']}
                        width={(Dimensions.get("window").width) * 0.33} // from react-native
                        height={100}
                        chartConfig={chartConfig}
                        hideLegend={true}
                        style={{ paddingLeft: 12 }}
                      />
                    </View>
                    <View style={{ textAlign: 'left' }}>
                      <Text style={styles.subTitles2}>Body</Text>
                      <ProgressChart
                        data={this.state['bodyData']}
                        width={(Dimensions.get("window").width) * 0.33} // from react-native
                        height={100}
                        chartConfig={chartConfig}
                        hideLegend={true}
                        style={{ paddingLeft: 12 }}
                      />
                    </View>
                  </View>
                </View>
                <View style={{ flexDirection: 'row', alignItems: 'flex-end', zIndex: 5, position: 'absolute', top: '50%', left: '40%' }}>
                  <Image style={{ width: 300 }} source={require('./assets/arm.png')} />
                </View>
                <View style={{ flexDirection: 'row', alignItems: 'flex-end', zIndex: 10, position: 'absolute', top: '60.5%', left: '56.5%' }}>
                  <FadeInView></FadeInView>
                  {/* <Animated.Image style={{ width: 100}} source={require('./assets/hand.png')} /> */}
                </View>
                <View style={{ paddingTop: 35 }}>
                  <Spinner></Spinner>
                </View>
              </LinearGradient>
            </View>
          </ThemeContext.Provider>
        }
      </>
    );
  }
};

const styles = StyleSheet.create({
  scrollView: {
    backgroundColor: Colors.lighter,
  },
  engine: {
    position: 'absolute',
    right: 0,
  },
  body: {
    backgroundColor: Colors.white,
  },
  sectionContainer: {
    marginTop: 32,
    paddingHorizontal: 24,
  },
  sectionTitle: {
    fontSize: 24,
    fontWeight: '600',
    color: Colors.black,
  },
  sectionDescription: {
    marginTop: 8,
    fontSize: 18,
    fontWeight: '400',
    color: Colors.dark,
  },
  highlight: {
    fontWeight: '700',
  },
  footer: {
    color: Colors.dark,
    fontSize: 12,
    fontWeight: '600',
    padding: 4,
    paddingRight: 12,
    textAlign: 'right',
  },
  floatView: {
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 5 },
    shadowOpacity: 0.8,
    shadowRadius: 5,
    elevation: 25,
    marginLeft: 5,
    marginRight: 5,
    marginTop: 10,
    borderRadius: 20,
  },
  subTitles: {
    color: 'white',
    fontSize: 30,
    paddingLeft: 8,
  },
  subTitles2: {
    alignSelf: 'flex-start',
    color: 'white',
    fontSize: 30,
  }
});