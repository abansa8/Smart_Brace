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
    Text,
    StatusBar,
    Dimensions
} from 'react-native';

class SpinnerComponent extends Component {
    render() {
        return (
            <View style={styles.semiCircle}>
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
    }
})
export default SpinnerComponent;