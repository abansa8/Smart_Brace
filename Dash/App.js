/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow
 */

import React from 'react';
import {
  SafeAreaView,
  StyleSheet,
  ScrollView,
  View,
  Text,
  StatusBar,
  Dimensions
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

const dataPro = {
  data: [0.4]
}
const dataPro2 = {
  data: [0.8]
}

const App: () => React$Node = () => {
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
                    labels: ["January", "February", "March", "April", "May", "June"],
                    datasets: [
                      {
                        data: [
                          Math.random() * 100,
                          Math.random() * 100,
                          Math.random() * 100,
                          Math.random() * 100,
                          Math.random() * 100,
                          Math.random() * 100
                        ]
                      }
                    ]
                  }}
                  width={(Dimensions.get("window").width) * 0.95} // from react-native
                  height={220}
                  yAxisLabel="$"
                  yAxisSuffix="k"
                  chartConfig={{
                    decimalPlaces: 2, // optional, defaults to 2dp
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
                      r: "6",
                      strokeWidth: "2",
                      stroke: "#0d47a1"
                    }
                  }}
                  bezier
                  style={styles.floatView}
                />
              </View>
              <View style={{ flexDirection: 'row', width: '100%', justifyContent: 'space-between', paddingTop: 24 }}>
                <View style={{ textAlign: 'center' }}>
                  <Text style={styles.subTitles2}>Wrist</Text>
                  <ProgressChart
                    data={dataPro}
                    width={(Dimensions.get("window").width) * 0.33} // from react-native
                    height={100}
                    chartConfig={chartConfig}
                    hideLegend={true}
                    style={{ paddingLeft: 12 }}
                  />
                </View>
                <View style={{ textAlign: 'center' }}>
                  <Text style={styles.subTitles2}>Body</Text>
                  <ProgressChart
                    data={dataPro2}
                    width={(Dimensions.get("window").width) * 0.33} // from react-native
                    height={100}
                    chartConfig={chartConfig}
                    hideLegend={true}
                    style={{ paddingLeft: 12 }}
                  />
                </View>
              </View>
              <View style={{paddingTop: 30}}>
                <Spinner></Spinner>
              </View>
            </LinearGradient>
          </View>
        </ThemeContext.Provider>
      }
    </>
  );
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
    borderRadius: 20
  },
  subTitles: {
    color: 'white',
    fontSize: 20,
    paddingLeft: 8,
  },
  subTitles2: {
    alignSelf: 'center',
    color: 'white',
    fontSize: 20,
  }
});

export default App;
