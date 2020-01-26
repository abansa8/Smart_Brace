import React, { Component } from 'react';
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
import {
    SafeAreaView,
    StyleSheet,
    ScrollView,
    View,
    Image,
    Text,
    StatusBar,
    Dimensions
} from 'react-native';

class SpinnerComponent extends Component {
    render() {
        return (
            <View style={{flexDirection: 'row', width: '80%'}}>
                <Image source={require('./assets/Spinner.png')}></Image>
            </View>
        )
    }
}

const styles = StyleSheet.create({
    semiCircle: {
        height: 150,
        width: 300,
        borderTopLeftRadius: 200,
        borderTopRightRadius: 200,
        backgroundColor: '#002071',
    },
    image1:{
        alignSelf: 'center',
        paddingRight: 24
    },
    image:{
        alignSelf: 'center',
        paddingLeft: 24
    }
})
export default SpinnerComponent;