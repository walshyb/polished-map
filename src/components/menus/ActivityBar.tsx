import { MenuItem, Icon, Menu } from 'semantic-ui-react'
import { useState } from 'react';

export default function ActivityBar(
  { fileExplorerActive, setFileExplorerActive }: { fileExplorerActive: boolean; setFileExplorerActive: (fileExplorerActive: boolean) => void }
) {
  const [activeItem, setActiveItem] = useState<string>('');
  const handleItemClick = (name: string) => {

    // If curernt item is already active
    if (activeItem === name) {
      setFileExplorerActive(!fileExplorerActive);
    }

    setActiveItem(name);
  };

  return (
    <Menu
      icon='labeled'
      vertical
      floated={true}
      style={{ display: 'flex', height: '100%', marginTop: 0 }}
    >
      <MenuItem
        name='file explorer'
        active={activeItem === 'file explorer'}
        onClick={() => {handleItemClick('file explorer')}}
      >
        <Icon name='file' />
      </MenuItem>

      <MenuItem
        name='search'
        active={activeItem === 'search'}
        onClick={() => {handleItemClick('search')}}
      >
        <Icon name='search' />
      </MenuItem>
    </Menu>
  )
}
