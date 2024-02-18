import logo from './logo.svg';
import './App.css';
import Editor from './components/editor/Editor';
import Menu from './components/menus/Menu';

function App() {
  return (
    <div className="App">
      <Menu />
      <Editor />
    </div>
  );
}

export default App;
