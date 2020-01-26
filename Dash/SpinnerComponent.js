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
            <View style={{width: '100%', position: 'absolute', zIndex: 88, bottom: -80, right: -160,  alignSelf: 'center'}}>
                <Image source={require('./assets/Spinner.png')}></Image>
            </View>
        )
    }
}

const styles = StyleSheet.create({
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